#include "gc/pure_gc.hpp"
#include <cstring>
#include <cassert>


#define WEAK_CONTAINER (-1)
#define FREED_POINTER (-1)
#define CACHE_PARENT_BITS (PURE_GC_CACHE_BITS / 3)
#define CACHE_CHILD_BITS (PURE_GC_CACHE_BITS - CACHE_PARENT_BITS)
#define CACHE_PARENT_MASK ((1 << CACHE_PARENT_BITS) -1 )
#define CACHE_CHILD_MASK ((1 << CACHE_CHILD_BITS) -1 )
#define UNSET_MASK 0x80000000
#define UNSET_MASK_BIT(a) ((unsigned)(a)>>31)

/**
 * Insert a parent/child handle pair into cache
 * @param parent
 * @param child
 * @return insertion success
 */
bool dvm::gc::pure_gc::gc_context::cache_insert(int parent, int child) {
    int hash = (parent & CACHE_PARENT_MASK) << CACHE_CHILD_BITS | (child & CACHE_CHILD_MASK);
    struct gc_cache_node *cn = &cache[hash];
    cache_dirty = true;

    if (cn->parent == -1) {
        cn->parent = parent;
        cn->child = child;
        return true;
    }
    if (cn->parent == parent && (cn->child ^ child) == UNSET_MASK) {
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
int dvm::gc::pure_gc::gc_context::node_alloc(void *p) {
    struct gc_node *ret;
    if (free == -1) {
        int sz = size * 2;
        int i;
        if (sz == 0) {
            sz = 1024;
        }

        pool = (struct gc_node *) ::realloc(pool, sz * sizeof(struct gc_node));
        ret = pool + size;
        ret->u.n.children = nullptr;

        for (i = size + 1; i < sz; i++) {
            pool[i].u.free = i + 1;
            pool[i].mark = -1;
            pool[i].u.n.children = nullptr;
        }
        pool[sz - 1].u.free = -1;
        free = size + 1;
        size = sz;
    } else {
        ret = pool + free;
        free = pool[free].u.free;
    }
    ret->u.n.mem = p;
    ret->mark = 0;
    ret->u.n.finalizer = nullptr;
    if (ret->u.n.children != nullptr) {
        ret->u.n.children->number = 0;
    }
    return (int) (ret - pool);
}

/**
 * expand link table space . each node has a link tabl
 * @param old
 * @param sz
 * @return
 */
dvm::gc::pure_gc::gc_link *dvm::gc::pure_gc::gc_context::link_expand(struct gc_link *old, int sz) {
    struct gc_link *ret;
    if (old != nullptr) {
        sz += old->number;
        if ((sz ^ old->number) <= old->number) {
            return old;
        }
    }

    sz = sz * 2 - 1;

    ret = (struct gc_link *) ::realloc(old, sizeof(struct gc_link) + (sz - 1) * sizeof(int));
    if (old == nullptr) {
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
static int cache_node_cmp(const void *a, const void *b) {
    auto ca = (const struct dvm::gc::pure_gc::gc_cache_node *) a;
    auto cb = (const struct dvm::gc::pure_gc::gc_cache_node *) b;
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
void dvm::gc::pure_gc::gc_context::cache_flush() {
    int i;
    if (!cache_dirty)
        return;
    qsort(cache, PURE_GC_CACHE_SIZE, sizeof(struct gc_cache_node), cache_node_cmp);
    i = 0;
    while (i < PURE_GC_CACHE_SIZE) {
        int parent = cache[i].parent;
        struct gc_cache_node *head;
        struct gc_cache_node *next;
        struct gc_node *node = &pool[parent];
        struct gc_link *children;
        int sz;
        int j, k;

        if (parent == -1) {
            break;
        }

        head = &cache[i];
        next = head;
        sz = 0;

        while (next->parent == parent && i < PURE_GC_CACHE_SIZE) {
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
                } else if ((int) (child & ~UNSET_MASK) < children->children[j]) {
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
            } else if ((int) (child & ~UNSET_MASK) < children->children[j]) {
                assert(child >= 0);
                children->children[k] = child;
                head->parent = -1;
                ++head;
                --j;
            } else {
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

    cache_dirty = false;
}

/**
 * add an edge into (or delete from) graph
 * @param parent
 * @param child
 */
void dvm::gc::pure_gc::gc_context::node_add(int parent, int child) {
    while (!cache_insert(parent, child)) {
        cache_flush();
    }
}

/**
 * expand hash map space
 */
void dvm::gc::pure_gc::gc_context::map_expand() {
    struct gc_hash_node **table;
    int sz, i;
    if (map.size == 0) {
        sz = 1024;
    } else {
        sz = map.size * 2;
    }
    table = (struct gc_hash_node **) ::malloc(sz * sizeof(struct gc_hash_node *));
    memset(table, 0, sz * sizeof(struct gc_hash_node *));
    for (i = 0; i < map.size; i++) {
        struct gc_hash_node *t = map.table[i];
        while (t != nullptr) {
            struct gc_hash_node *tmp = t;
            void *p = pool[tmp->id].u.n.mem;
            int new_hash = hash(p) & (sz - 1);
            t = t->next;

            tmp->next = table[new_hash];
            table[new_hash] = tmp;
        }
    }

    ::free(map.table);
    map.table = table;
    map.size = sz;
}

/**
 * map a existed pointer into a handle , if not exist, create a new one
 * @param p
 * @return
 */
int dvm::gc::pure_gc::gc_context::map_id(void *p) {
    int h = hash(p);
    struct gc_hash_node *node = map.table[h & (map.size - 1)];
    while (node != nullptr) {
        if (pool[node->id].u.n.mem == p) {
            return node->id;
        }
        node = node->next;
    }
    if (map.number >= map.size) {
        map_expand();
    }

    ++map.number;

    if (map.free != nullptr) {
        node = map.free;
        map.free = node->next;
    } else {
        node = (struct gc_hash_node *) ::malloc(sizeof(*node));
    }
    node->id = node_alloc(p);
    node->next = map.table[h & (map.size - 1)];
    map.table[h & (map.size - 1)] = node;

    return node->id;
}

/**
 * Erase a handle from hash map
 * @param id
 */
void dvm::gc::pure_gc::gc_context::map_erase(int id) {
    void *p = pool[id].u.n.mem;
    if (p != nullptr) {
        int h = hash(p);
        struct gc_hash_node **node = &map.table[h & (map.size - 1)];
        struct gc_hash_node *find;
        while ((*node)->id != id) {
            node = &(*node)->next;
            assert(*node);
        }
        find = *node;
        *node = find->next;
        find->next = map.free;
        map.free = find;
        --map.number;
    }
}

/**
 * Push a handle into the stack
 * @param handle
 */
void dvm::gc::pure_gc::gc_context::stack_push(int handle) {
    stack_expand();
    stack.data[stack.top++].handle = handle;
}

/**
 *  gc brackets open
 */
void dvm::gc::pure_gc::gc_context::enter() {
    stack_expand();
    stack.data[stack.top].number = stack.top - stack.current;
    stack.current = stack.top++;
}

/**
 * gc brackets close , extend some pointers' lifetime
 * @param p
 * @param ...
 */
void dvm::gc::pure_gc::gc_context::leave(void *p, ...) {
    if (stack.current >= stack.bottom) {
        stack.top = stack.current;
        stack.current -= stack.data[stack.current].number;
    } else {
        --stack.bottom;
        int parent = stack.data[stack.bottom - 1].stack;
        int child = stack.data[stack.bottom].stack;

        node_add(parent, child | UNSET_MASK);
        node_free(child);
        stack.current = stack.bottom - 1;
        stack.top = stack.current + 1;
    }

    // TODO: Fix double free
    void **head = &p;

    while (*head != nullptr) {
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
int dvm::gc::pure_gc::gc_context::stack_pack_internal(int from, int to, int top) {
    if (to < from) {
        int parent = stack.data[to].stack;
        while (from < top) {
            node_add(parent, stack.data[from].handle);
            ++from;
        }
        return to + 1;
    }

    int bottom = stack_pack_internal(from, to - stack.data[to].number, to);
    int node = node_alloc(nullptr);
    ++to;
    while (to < top) {
        node_add(node, stack.data[to].handle);
        ++to;
    }
    node_add(stack.data[bottom - 1].stack, node);
    stack.data[bottom].stack = node;
    return bottom + 1;
}

/**
 * pack the value in the stack
 */
void dvm::gc::pure_gc::gc_context::stack_pack() {
    int bottom = stack_pack_internal(stack.bottom, stack.current, stack.top);
    stack.top = stack.bottom = bottom;
    stack.current = bottom - 1;
}

/**
 * link or unlink two pointer
 * @param parent
 * @param prev
 * @param now
 */
void dvm::gc::pure_gc::gc_context::link(void *parent, void *prev, void *now) {
    int parent_id;
    if (parent == nullptr) {
        parent_id = 0;
    } else {
        parent_id = map_id(parent);
    }
    if (prev != nullptr) {
        int prev_id = map_id(prev);
        stack_push(prev_id);
        node_add(parent_id, prev_id | UNSET_MASK);
    }
    if (now != nullptr) {
        node_add(parent_id, map_id(now));
    }
}

/**
 * init struct E
 */
void dvm::gc::pure_gc::gc_context::gc_init() {
    int i;
    int root;

    pool = nullptr;
    size = 0;
    mark = 1;
    free = -1;
    cache_dirty = false;
    for (i = 0; i < PURE_GC_CACHE_SIZE; i++) {
        cache[i].parent = -1;
    }
    stack.data = nullptr;
    stack.top = 0;

    root = node_alloc(nullptr);
    assert(root == 0);
    stack_expand();
    stack.data[stack.top++].stack = root;

    stack.bottom = stack.top;
    stack.current = stack.bottom - 1;

    map.table = nullptr;
    map.size = 0;
    map.free = nullptr;
    map.number = 0;

    map_expand();
}

/**
 * release all the resource used in gc
 */
void dvm::gc::pure_gc::gc_context::gc_exit() {
    int i;
    for (i = 0; i < size; i++) {
        ::free(pool[i].u.n.children);
        if (pool[i].mark >= 0) {
            void *p = pool[i].u.n.mem;
            if ((pool[i].u.n.finalizer != nullptr) && pool[i].u.c.weak != WEAK_CONTAINER) {
                pool[i].u.n.finalizer(p);
            }
            if ((intptr_t) p != FREED_POINTER) {
                ::free(p);
            }
        }
    }
    ::free(pool);
    for (i = 0; i < map.size; i++) {
        struct gc_hash_node *p = map.table[i];
        while (p != nullptr) {
            struct gc_hash_node *n = p->next;
            ::free(p);
            p = n;
        }
    }
    ::free(map.table);
    while (map.free != nullptr) {
        struct gc_hash_node *p = map.free->next;
        ::free(map.free);
        map.free = p;
    }
    ::free(stack.data);
    map.number = 0;
}

/**
 * mark and sweep
 * @param root
 */
void dvm::gc::pure_gc::gc_context::gc_mark(int root) {
    if (pool[root].mark < mark + 1) {
        struct gc_link *children = pool[root].u.n.children;
        pool[root].mark = mark + 1;
        if (children != nullptr) {
            int i;
            if (pool[root].u.c.weak == WEAK_CONTAINER) {
                bool merge = false;
                for (i = children->number - 1; i >= 0; i--) {
                    int child = children->children[i];
                    if ((intptr_t) pool[child].u.n.mem == FREED_POINTER) {
                        children->children[i] = 0;
                        merge = true;
                    } else
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
            } else {
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
void dvm::gc::pure_gc::gc_context::collect() {
    int i;
    stack_pack();
    cache_flush();
    gc_mark(0);
    for (i = 0; i < size; i++) {
        if (pool[i].mark < mark) {
            if (pool[i].mark >= 0) {
                void *p = pool[i].u.n.mem;
                if ((pool[i].u.n.finalizer != nullptr) && pool[i].u.c.weak != WEAK_CONTAINER) {
                    pool[i].u.n.finalizer(p);
                }
                if ((intptr_t) p != FREED_POINTER) {
                    ::free(p);
                    map_erase(i);
                }
                node_free(i);
            }
        } else if (pool[i].mark == mark) {
            void *p = pool[i].u.n.mem;
            if ((pool[i].u.n.finalizer != nullptr) && pool[i].u.c.weak != WEAK_CONTAINER) {
                pool[i].u.n.finalizer(p);
                pool[i].u.n.finalizer = nullptr;
            }
            ::free(p);
            map_erase(i);
            pool[i].u.c.mem = FREED_POINTER;
        }
    }
    mark += 2;
}


/**
 * allocate a memory block , and create a node map to it. node will link to parent
 * @param sz
 * @param parent
 * @param finalizer
 * @return
 */
void *dvm::gc::pure_gc::gc_context::malloc(size_t sz, void *parent, void (*finalizer)(void *)) {
    void *ret = ::malloc(sz);
    int id = map_id(ret);
    pool[id].u.n.finalizer = finalizer;
    if (parent != nullptr) {
        link(parent, nullptr, ret);
    } else {
        stack_push(id);
    }
    return ret;
}

/**
 * allocate a weak pointer container
 * @param parent
 * @return
 */
dvm::gc::pure_gc::gc_weak_table *dvm::gc::pure_gc::gc_context::new_weak_table(void *parent) {
    gc_weak_table *ret = (struct gc_weak_table *) ::malloc(sizeof(*ret));
    ret->node_id = map_id(ret);
    pool[ret->node_id].u.c.weak = WEAK_CONTAINER;
    if (parent != nullptr) {
        link(parent, nullptr, ret);
    } else {
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
void *dvm::gc::pure_gc::gc_context::weak_table_next(struct gc_weak_table *cont, int *iter) {
    int i, j;
    struct gc_link *children;
    cache_flush();
    children = pool[cont->node_id].u.n.children;
    if (children == nullptr) {
        return nullptr;
    }

    for (i = (iter == nullptr ? 0 : *iter); i < children->number; i++) {
        int id = children->children[i];
        if (id != 0) {
            if (pool[id].u.c.mem == FREED_POINTER) {
                children->children[i] = 0;
            } else {
                if (iter != nullptr) {
                    *iter = i + 1;
                }
                stack_push(id);
                return pool[id].u.n.mem;
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

    return nullptr;
}

/**
 * clone a memory block , this will copy all the edges linked to orginal node
 * @param from
 * @param sz
 * @return
 */
void *dvm::gc::pure_gc::gc_context::clone(void *from, size_t sz) {
    int from_id = map_id(from);
    void *to = ::malloc(sz);
    int to_id = map_id(to);
    struct gc_link *from_children = pool[from_id].u.n.children;
    stack_push(to_id);

    cache_flush();
    memcpy(to, from, sz);
    pool[to_id].u.n.finalizer = pool[from_id].u.n.finalizer;
    pool[to_id].u.n.children = link_expand(pool[to_id].u.n.children, from_children->number);
    memcpy(pool[to_id].u.n.children, from_children,
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
void *dvm::gc::pure_gc::gc_context::realloc(void *p, size_t sz, void *parent) {
    void *ret;
    assert(sz > 0);

    if (p == nullptr) {
        return malloc(sz, parent, nullptr);
    }

    ret = ::realloc(p, sz);
    if (ret == p) {
        return ret;
    }

    int new_id = map_id(ret);
    int old_id = map_id(p);

    struct gc_link *tmp = pool[new_id].u.n.children;
    pool[new_id].u.n.children = pool[old_id].u.n.children;
    pool[old_id].u.n.children = tmp;

    pool[new_id].u.n.finalizer = pool[old_id].u.n.finalizer;

    if (parent != nullptr) {
        link(parent, p, ret);
    } else {
        stack_push(new_id);
    }

    map_erase(old_id);
    pool[old_id].u.c.mem = FREED_POINTER;


    return ret;
}

dvm::gc::pure_gc::gc_context::~gc_context() {
    gc_exit();
}

dvm::gc::pure_gc::gc_context::gc_context() {
    gc_init();
}

