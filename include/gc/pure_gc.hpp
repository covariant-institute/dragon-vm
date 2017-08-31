#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdlib>

#define PURE_GC_CACHE_BITS 10
#define PURE_GC_CACHE_SIZE (1 << PURE_GC_CACHE_BITS)

namespace dvm {
    namespace gc {
        namespace pure_gc {
            struct gc_link {
                int number;
                int children[1];
            };

            struct gc_node {
                int mark;
                union {
                    struct {
                        void *mem;
                        struct gc_link *children;

                        void (*finalizer)(void *);
                    } n;

                    struct {
                        intptr_t mem;
                        struct gc_link *children;
                        intptr_t weak;
                    } c;
                    int free;
                } u;
            };

            union gc_stack_node {
                int stack;
                int number;
                int handle;
            };

            struct gc_stack {
                union gc_stack_node *data;
                int top;
                int bottom;
                int current;
            };

            struct gc_hash_node {
                int id;
                struct gc_hash_node *next;
            };

            struct gc_hash_map {
                struct gc_hash_node **table;
                int size;
                struct gc_hash_node *free;
                int number;
            };

            struct gc_cache_node {
                int parent;
                int child;
            };

            struct gc_weak_table {
                int node_id;
            };

            class gc_context {
            private:
                int size;
                int free;
                int mark;
                bool cache_dirty;

                gc_node *pool;
                gc_stack stack;
                gc_hash_map map;
                gc_cache_node cache[PURE_GC_CACHE_SIZE];

                bool cache_insert(int parent, int child);

                int node_alloc(void *p);

                gc_link *link_expand(struct gc_link *old, int sz);

                int stack_pack_internal(int from, int to, int top);

                void stack_pack();

                void gc_mark(int root);

                void cache_flush();

                void node_add(int parent, int child);

                void map_expand();

                int map_id(void *p);

                void map_erase(int id);

                void gc_init();

                void gc_exit();

                void stack_push(int handle);

                /**
                 * free a node for reuse
                 * @param id
                 */
                inline void node_free(int id) {
                    pool[id].mark = -1;
                    if (pool[id].u.n.children != nullptr) {
                        pool[id].u.n.children->number = 0;
                    }
                    pool[id].u.free = free;
                    free = id;
                }

                /**
                 * create a hash value for pointer p
                 * @param p
                 * @return
                 */
                inline int hash(void *p) {
                    auto t = (intptr_t) p;
                    return (int) ((t >> 2) ^ (t >> 16));
                }

                /**
                 * Expand stack space
                 */
                inline void stack_expand() {
                    if (((stack.top + 1) ^ stack.top) > stack.top) {
                        stack.data = (union gc_stack_node *) ::realloc(stack.data,
                                                                    (stack.top * 2 + 1) * sizeof(union gc_stack_node));
                    }
                }

                /**
                 * mark the nodes related to root
                 * @param weak
                 */
                inline void gc_mark_weak(int weak) {
                    if (pool[weak].mark < mark) {
                        pool[weak].mark = mark;
                    }
                }

            public:
                gc_context(const gc_context &) = delete;

                gc_context();

                ~gc_context();

                void enter();

                void leave(void *p, ...);

                void link(void *parent, void *prev, void *now);

                void collect();

                void *malloc(size_t sz, void *parent, void (*finalizer)(void *));

                void *clone(void *from, size_t sz);

                void *realloc(void *p, size_t sz, void *parent);

                struct gc_weak_table *new_weak_table(void *parent);

                void *weak_table_next(struct gc_weak_table *cont, int *iter);
            };
        }
    }
}

