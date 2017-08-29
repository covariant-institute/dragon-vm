#include "gc/gc.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cassert>

#define my_malloc malloc
#define my_free free
#define my_realloc realloc

#define CACHE_BITS 10

struct link {
	int number;
	int children[1];
};

struct node {
	int mark;
	union {
		struct {
			void *mem;
			struct link *children;

			void (*finalizer)(void *);
		} n;

		struct {
			intptr_t mem;
			struct link *children;
			intptr_t weak;
		} c;
		int free;
	} u;
};

/*	   stack data
	  +----------+
	0 | level 0  |                ----> level 0 / root ( node pack )
	1 | level 1  |                --+-> level 1 ( 1 node ref + node pack )
	2 | node ref | <-bottom       --+
	3 | 2 (lv.2) |
	4 | node ref |                --+-> level 2 ( 3 node ref )
	5 | node ref |                  |
	6 | node ref |                --+
	7 | 4 (lv.3) | <-current
	8 | node ref |                --+-> level 3 ( 2 node ref )
	9 | node ref |                --+
	10|   nil    | <-top
	11|   nil    |
	  +----------+
 */

union stack_node {
	int stack;
	int number;
	int handle;
};

struct stack {
	union stack_node *data;
	int top;
	int bottom;
	int current;
};

struct hash_node {
	int id;
	struct hash_node *next;
};

struct hash_map {
	struct hash_node **table;
	int size;
	struct hash_node *free;
	int number;
};

#define    CACHE_SIZE (1<<CACHE_BITS)

struct cache_node {
	int parent;
	int child;
};

static struct {
	struct node *pool;
	int size;
	int free;
	int mark;
	bool cache_dirty;
	struct stack stack;
	struct hash_map map;
	struct cache_node cache[CACHE_SIZE];
} E;

#define WEAK_CONTAINER -1
#define FREED_POINTER -1
#define CACHE_PARENT_BITS (CACHE_BITS/3)
#define CACHE_CHILD_BITS (CACHE_BITS-CACHE_PARENT_BITS)
#define CACHE_PARENT_MASK ((1<<CACHE_PARENT_BITS) -1 )
#define CACHE_CHILD_MASK ((1<<CACHE_CHILD_BITS) -1 )
#define UNSET_MASK 0x80000000
#define UNSET_MASK_BIT(a) ((unsigned)(a)>>31)

/**
 * Insert a parent/child handle pair into cache
 * @param parent
 * @param child
 * @return insertion success
 */
static bool cache_insert(int parent, int child)
{
	int hash = (parent & CACHE_PARENT_MASK) << CACHE_CHILD_BITS | (child & CACHE_CHILD_MASK);
	struct cache_node *cn = &E.cache[hash];
	E.cache_dirty = true;

	if (cn->parent == -1) {
		cn->parent = parent;
		cn->child = child;
		return true;
	}
	else if (cn->parent == parent && (cn->child ^ child) == UNSET_MASK) {
		cn->parent = -1;
		return true;
	}
	return false;
}

/**
 * create a new handle for pointer p
 * @param p pointer
 * @return handle
 */
static int node_alloc(void *p)
{
	struct node *ret;
	if (E.free == -1) {
		int sz = E.size * 2;
		int i;
		if (sz == 0) {
			sz = 1024;
		}

		E.pool = (struct node *) my_realloc(E.pool, sz * sizeof(struct node));
		ret = E.pool + E.size;
		ret->u.n.children = 0;

		for (i = E.size + 1; i < sz; i++) {
			E.pool[i].u.free = i + 1;
			E.pool[i].mark = -1;
			E.pool[i].u.n.children = 0;
		}
		E.pool[sz - 1].u.free = -1;
		E.free = E.size + 1;
		E.size = sz;
	}
	else {
		ret = E.pool + E.free;
		E.free = E.pool[E.free].u.free;
	}
	ret->u.n.mem = p;
	ret->mark = 0;
	ret->u.n.finalizer = 0;
	if (ret->u.n.children != nullptr) {
		ret->u.n.children->number = 0;
	}
	return (int) (ret - E.pool);
}

/**
 * expand link table space . each node has a link table.
 * @param old
 * @param sz
 * @return
 */
static struct link *link_expand(struct link *old, int sz)
{
	struct link *ret;
	if (old != 0) {
		sz += old->number;
		if ((sz ^ old->number) <= old->number) {
			return old;
		}
	}

	sz = sz * 2 - 1;

	ret = (struct link *) my_realloc(old, sizeof(struct link) + (sz - 1) * sizeof(int));
	if (old == 0) {
		ret->number = 0;
	}
	return ret;
}

/**
 * comparator for cache sort
 * @param a
 * @param b
 * @return
 */
static int cache_node_cmp(const void *a, const void *b)
{
	const struct cache_node *ca = (const struct cache_node *) a;
	const struct cache_node *cb = (const struct cache_node *) b;
	if (ca->parent != cb->parent) {
		return cb->parent - ca->parent;
	}
	if (ca->parent == -1) {
		return 0;
	}
	return (ca->child & ~UNSET_MASK) - (cb->child & ~UNSET_MASK);
}


/**
 * commit cache to the node graph
 */
static void cache_flush()
{
	int i;
	if (!E.cache_dirty)
		return;
	qsort(E.cache, CACHE_SIZE, sizeof(struct cache_node), cache_node_cmp);
	i = 0;
	while (i < CACHE_SIZE) {
		int parent = E.cache[i].parent;
		struct cache_node *head;
		struct cache_node *next;
		struct node *node = &E.pool[parent];
		struct link *children;
		int sz;
		int j, k;

		if (parent == -1) {
			break;
		}

		head = &E.cache[i];
		next = head;
		sz = 0;

		while (next->parent == parent && i < CACHE_SIZE) {
			sz += 1 - UNSET_MASK_BIT(next->child);
			++next;
			++i;
		}

		children = node->u.n.children;

		k = j = 0;

		if (children != nullptr) {
			while (j < children->number) {
				int child;
				if (head >= next) {
					goto copy_next;
				}

				child = head->child;
				children->children[k] = children->children[j];
				if (child == (int) ((children->children[j] | UNSET_MASK))) {
					--k;
					head->parent = -1;
					--sz;
					++head;
				}
				else if ((int) (child & ~UNSET_MASK) < children->children[j]) {
					break;
				}
				++j;
				++k;
			}
		}
		if (sz > 0) {
			children = node->u.n.children = link_expand(node->u.n.children, sz);
			assert(children);
			memmove(children->children + j + sz, children->children + j, (children->number - j) * sizeof(int));
			j += sz;
			children->number += sz;
		}


		while (j < children->number) {
			int child;
			if (head >= next) {
				goto copy_next;
			}

			child = head->child;
			if (child == (int) (children->children[j] | UNSET_MASK)) {
				--k;
				head->parent = -1;
				++head;
			}
			else if ((int) (child & ~UNSET_MASK) < children->children[j]) {
				assert(child >= 0);
				children->children[k] = child;
				head->parent = -1;
				++head;
				--j;
			}
			else {
				children->children[k] = children->children[j];
			}
			++j;
			++k;
		}
		while (head < next) {
			int child = head->child;
			assert((child & UNSET_MASK) == 0);
			children->children[k] = child;
			++k;
			head->parent = -1;
			++head;
		}
		children->number = k;
		continue;
copy_next:
		if (k != j) {
			for (; j < children->number; j++, k++) {
				children->children[k] = children->children[j];
			}
			children->number = k;
		}
	}

	E.cache_dirty = false;
}

/**
 * add an edge into (or delete from) graph
 * @param parent
 * @param child
 */
static void node_add(int parent, int child)
{
	while (!cache_insert(parent, child)) {
		cache_flush();
	}
}

/**
 * free a node for reuse
 * @param id
 */
static __inline void node_free(int id)
{
	E.pool[id].mark = -1;
	if (E.pool[id].u.n.children) {
		E.pool[id].u.n.children->number = 0;
	}
	E.pool[id].u.free = E.free;
	E.free = id;
}

/**
 * create a hash value for pointer p
 * @param p
 * @return
 */
static __inline int hash(void *p)
{
	intptr_t t = (intptr_t) p;
	return (int) ((t >> 2) ^ (t >> 16));
}

/**
 * expand hash map space
 */
static void map_expand()
{
	struct hash_node **table;
	int sz, i;
	if (E.map.size == 0) {
		sz = 1024;
	}
	else {
		sz = E.map.size * 2;
	}
	table = (struct hash_node **) my_malloc(sz * sizeof(struct hash_node *));
	memset(table, 0, sz * sizeof(struct hash_node *));
	for (i = 0; i < E.map.size; i++) {
		struct hash_node *t = E.map.table[i];
		while (t) {
			struct hash_node *tmp = t;
			void *p = E.pool[tmp->id].u.n.mem;
			int new_hash = hash(p) & (sz - 1);
			t = t->next;

			tmp->next = table[new_hash];
			table[new_hash] = tmp;
		}
	}

	my_free(E.map.table);
	E.map.table = table;
	E.map.size = sz;
}

/**
 * map a existed pointer into a handle , if not exist, create a new one
 * @param p
 * @return
 */
static int map_id(void *p)
{
	int h = hash(p);
	struct hash_node *node = E.map.table[h & (E.map.size - 1)];
	while (node) {
		if (E.pool[node->id].u.n.mem == p) {
			return node->id;
		}
		node = node->next;
	}
	if (E.map.number >= E.map.size) {
		map_expand();
	}

	++E.map.number;

	if (E.map.free) {
		node = E.map.free;
		E.map.free = node->next;
	}
	else {
		node = (struct hash_node *) my_malloc(sizeof(*node));
	}
	node->id = node_alloc(p);
	node->next = E.map.table[h & (E.map.size - 1)];
	E.map.table[h & (E.map.size - 1)] = node;

	return node->id;
}

/**
 * Erase a handle from hash map
 * @param id
 */
static void map_erase(int id)
{
	void *p = E.pool[id].u.n.mem;
	if (p) {
		int h = hash(p);
		struct hash_node **node = &E.map.table[h & (E.map.size - 1)];
		struct hash_node *find;
		while ((*node)->id != id) {
			node = &(*node)->next;
			assert(*node);
		}
		find = *node;
		*node = find->next;
		find->next = E.map.free;
		E.map.free = find;
		--E.map.number;
	}
}

/**
 * Expand stack space
 */
static __inline void stack_expand()
{
	if (((E.stack.top + 1) ^ E.stack.top) > E.stack.top) {
		E.stack.data = (union stack_node *) my_realloc(E.stack.data, (E.stack.top * 2 + 1) * sizeof(union stack_node));
	}
}

/**
 * Push a handle into the stack
 * @param handle
 */
static void stack_push(int handle)
{
	stack_expand();
	E.stack.data[E.stack.top++].handle = handle;
}

/**
 *  gc brackets open
 */
void gc_enter()
{
	stack_expand();
	E.stack.data[E.stack.top].number = E.stack.top - E.stack.current;
	E.stack.current = E.stack.top++;
}

/**
 * gc brackets close , extend some pointers' lifetime
 * @param p
 * @param ...
 */
void gc_leave(void *p, ...)
{
	void **head;
	if (E.stack.current >= E.stack.bottom) {
		E.stack.top = E.stack.current;
		E.stack.current -= E.stack.data[E.stack.current].number;
	}
	else {
		int parent, child;
		--E.stack.bottom;
		parent = E.stack.data[E.stack.bottom - 1].stack;
		child = E.stack.data[E.stack.bottom].stack;
		node_add(parent, child | UNSET_MASK);
		node_free(child);
		E.stack.current = E.stack.bottom - 1;
		E.stack.top = E.stack.current + 1;
	}

	head = &p;

	while (*head) {
		stack_push(map_id(*head));
		++head;
	}
}

/**
 * pack the stack recursively
 * @param from
 * @param to
 * @param top
 * @return
 */
static int stack_pack_internal(int from, int to, int top)
{
	if (to < from) {
		int parent = E.stack.data[to].stack;
		while (from < top) {
			node_add(parent, E.stack.data[from].handle);
			++from;
		}
		return to + 1;
	}
	else {
		int bottom = stack_pack_internal(from, to - E.stack.data[to].number, to);
		int node = node_alloc(0);
		++to;
		while (to < top) {
			node_add(node, E.stack.data[to].handle);
			++to;
		}
		node_add(E.stack.data[bottom - 1].stack, node);
		E.stack.data[bottom].stack = node;
		return bottom + 1;
	}
}

/**
 * pack the value in the stack
 */
static void stack_pack()
{
	int bottom = stack_pack_internal(E.stack.bottom, E.stack.current, E.stack.top);
	E.stack.top = E.stack.bottom = bottom;
	E.stack.current = bottom - 1;
}

/**
 * link or unlink two pointer
 * @param parent
 * @param prev
 * @param now
 */
void gc_link(void *parent, void *prev, void *now)
{
	int parent_id;
	if (parent == 0) {
		parent_id = 0;
	}
	else {
		parent_id = map_id(parent);
	}
	if (prev) {
		int prev_id = map_id(prev);
		stack_push(prev_id);
		node_add(parent_id, prev_id | UNSET_MASK);
	}
	if (now) {
		node_add(parent_id, map_id(now));
	}
}

/**
 * init struct E
 */
void gc_init()
{
	int i;
	int root;

	E.pool = 0;
	E.size = 0;
	E.mark = 1;
	E.free = -1;
	E.cache_dirty = false;
	for (i = 0; i < CACHE_SIZE; i++) {
		E.cache[i].parent = -1;
	}
	E.stack.data = 0;
	E.stack.top = 0;

	root = node_alloc(0);
	assert(root == 0);
	stack_expand();
	E.stack.data[E.stack.top++].stack = root;

	E.stack.bottom = E.stack.top;
	E.stack.current = E.stack.bottom - 1;

	E.map.table = 0;
	E.map.size = 0;
	E.map.free = 0;
	E.map.number = 0;

	map_expand();
}

/**
 * release all the resource used in gc
 */
void gc_exit()
{
	int i;
	for (i = 0; i < E.size; i++) {
		my_free(E.pool[i].u.n.children);
		if (E.pool[i].mark >= 0) {
			void *p = E.pool[i].u.n.mem;
			if (E.pool[i].u.n.finalizer && E.pool[i].u.c.weak != WEAK_CONTAINER) {
				E.pool[i].u.n.finalizer(p);
			}
			if ((intptr_t) p != FREED_POINTER) {
				my_free(p);
			}
		}
	}
	my_free(E.pool);
	for (i = 0; i < E.map.size; i++) {
		struct hash_node *p = E.map.table[i];
		while (p) {
			struct hash_node *n = p->next;
			my_free(p);
			p = n;
		}
	}
	my_free(E.map.table);
	while (E.map.free) {
		struct hash_node *p = E.map.free->next;
		my_free(E.map.free);
		E.map.free = p;
	}
	my_free(E.stack.data);
	E.map.number = 0;
}

/**
 * mark the nodes related to root
 * @param weak
 */
static __inline void gc_mark_weak(int weak)
{
	if (E.pool[weak].mark < E.mark) {
		E.pool[weak].mark = E.mark;
	}
}

/**
 * mark and sweep
 * @param root
 */
static void gc_mark(int root)
{
	if (E.pool[root].mark < E.mark + 1) {
		struct link *children = E.pool[root].u.n.children;
		E.pool[root].mark = E.mark + 1;
		if (children) {
			int i;
			if (E.pool[root].u.c.weak == WEAK_CONTAINER) {
				bool merge = false;
				for (i = children->number - 1; i >= 0; i--) {
					int child = children->children[i];
					if ((intptr_t) E.pool[child].u.n.mem == FREED_POINTER) {
						children->children[i] = 0;
						merge = true;
					}
					else
						gc_mark_weak(child);
				}

				if (merge) {
					int j;
					for (i = 0; i < children->number; i++) {
						if (children->children[i] == 0) {
							break;
						}
					}

					for (j = i, ++i; i < children->number; i++) {
						if (children->children[i] != 0) {
							children->children[j++] = children->children[i];
						}
					}

					children->number = j;
				}
			}
			else {
				for (i = children->number - 1; i >= 0; i--) {
					gc_mark(children->children[i]);
				}
			}
		}
	}
}

/**
 * collect the memory block can no longer be otherwise accessed
 */
void gc_collect()
{
	int i;
	stack_pack();
	cache_flush();
	gc_mark(0);
	for (i = 0; i < E.size; i++) {
		if (E.pool[i].mark < E.mark) {
			if (E.pool[i].mark >= 0) {
				void *p = E.pool[i].u.n.mem;
				if (E.pool[i].u.n.finalizer && E.pool[i].u.c.weak != WEAK_CONTAINER) {
					E.pool[i].u.n.finalizer(p);
				}
				if ((intptr_t) p != FREED_POINTER) {
					my_free(p);
					map_erase(i);
				}
				node_free(i);
			}
		}
		else if (E.pool[i].mark == E.mark) {
			void *p = E.pool[i].u.n.mem;
			if (E.pool[i].u.n.finalizer && E.pool[i].u.c.weak != WEAK_CONTAINER) {
				E.pool[i].u.n.finalizer(p);
				E.pool[i].u.n.finalizer = 0;
			}
			my_free(p);
			map_erase(i);
			E.pool[i].u.c.mem = FREED_POINTER;
		}
	}
	E.mark += 2;
}

/**
 * only for debug, print all the relationship of all nodes
 */
void gc_debug_print_nodes()
{
	int i;
	printf("------gc debug begin----\n");
	stack_pack();
	cache_flush();
	gc_mark(0);
	for (i = 0; i < E.size; i++) {
		struct link *link = E.pool[i].u.n.children;

		if (E.pool[i].mark >= E.mark + 1) {
			if (E.pool[i].u.c.weak == WEAK_CONTAINER) {
				printf("%d[weak] -> ", i);
			}
			else {
				printf("%d(%p) -> ", i, E.pool[i].u.n.mem);
			}
		}
		else if (E.pool[i].mark == E.mark) {
			printf("w %d: ", i);
		}
		else if (E.pool[i].mark >= 0) {
			printf("x %d(%p): ", i, E.pool[i].u.n.mem);
		}
		else {
			continue;
		}

		if (link) {
			int j;
			for (j = 0; j < link->number; j++) {
				printf("%d,", link->children[j]);
			}
		}

		printf("\n");
	}
	E.mark++;
	printf("------gc debug end------\n");
}

/**
 * allocate a memory block , and create a node map to it. node will link to parent
 * @param sz
 * @param parent
 * @param finalizer
 * @return
 */
void *gc_malloc(size_t sz, void *parent, void (*finalizer)(void *))
{
	void *ret = my_malloc(sz);
	int id = map_id(ret);
	E.pool[id].u.n.finalizer = finalizer;
	if (parent) {
		gc_link(parent, 0, ret);
	}
	else {
		stack_push(id);
	}
	return ret;
}

struct gc_weak_table {
	int node_id;
};

/**
 * allocate a weak pointer container
 * @param parent
 * @return
 */
struct gc_weak_table *gc_weak_table(void *parent)
{
	struct gc_weak_table *ret = (struct gc_weak_table *) my_malloc(sizeof(*ret));
	ret->node_id = map_id(ret);
	E.pool[ret->node_id].u.c.weak = WEAK_CONTAINER;
	if (parent) {
		gc_link(parent, 0, ret);
	}
	else {
		stack_push(ret->node_id);
	}
	return ret;
}

/**
 * iterate the weak container
 * @param cont
 * @param iter
 * @return
 */
void *gc_weak_next(struct gc_weak_table *cont, int *iter)
{
	int i, j;
	struct link *children;
	cache_flush();
	children = E.pool[cont->node_id].u.n.children;
	if (children == 0) {
		return 0;
	}

	for (i = (iter == 0 ? 0 : *iter); i < children->number; i++) {
		int id = children->children[i];
		if (id) {
			if (E.pool[id].u.c.mem == FREED_POINTER) {
				children->children[i] = 0;
			}
			else {
				if (iter) {
					*iter = i + 1;
				}
				stack_push(id);
				return E.pool[id].u.n.mem;
			}
		}
	}

	for (i = 0; i < children->number; i++) {
		if (children->children[i] == 0) {
			break;
		}
	}

	for (j = i, ++i; i < children->number; i++) {
		if (children->children[i] != 0) {
			children->children[j++] = children->children[i];
		}
	}

	children->number = j;

	return 0;
}

/**
 * clone a memory block , this will copy all the edges linked to orginal node
 * @param from
 * @param sz
 * @return
 */
void *gc_clone(void *from, size_t sz)
{
	int from_id = map_id(from);
	void *to = my_malloc(sz);
	int to_id = map_id(to);
	struct link *from_children = E.pool[from_id].u.n.children;
	stack_push(to_id);

	cache_flush();
	memcpy(to, from, sz);
	E.pool[to_id].u.n.finalizer = E.pool[from_id].u.n.finalizer;
	E.pool[to_id].u.n.children = link_expand(E.pool[to_id].u.n.children, from_children->number);
	memcpy(E.pool[to_id].u.n.children, from_children,
	       sizeof(*from_children) + (from_children->number - 1) * sizeof(int));
	return to;
}

/**
 * realloc a memory block , all the edages linked to it will be retained
 * @param p
 * @param sz
 * @param parent
 * @return
 */
void *gc_realloc(void *p, size_t sz, void *parent)
{
	void *ret;
	assert(sz > 0);

	if (p == 0) {
		return gc_malloc(sz, parent, 0);
	}

	ret = my_realloc(p, sz);
	if (ret == p) {
		return ret;
	}
	else {
		int new_id = map_id(ret);
		int old_id = map_id(p);

		struct link *tmp = E.pool[new_id].u.n.children;
		E.pool[new_id].u.n.children = E.pool[old_id].u.n.children;
		E.pool[old_id].u.n.children = tmp;

		E.pool[new_id].u.n.finalizer = E.pool[old_id].u.n.finalizer;

		if (parent) {
			gc_link(parent, p, ret);
		}
		else {
			stack_push(new_id);
		}

		map_erase(old_id);
		E.pool[old_id].u.c.mem = FREED_POINTER;
	}

	return ret;
}
