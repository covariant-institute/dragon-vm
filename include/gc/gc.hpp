#pragma once

#include <cstddef>

#ifdef __cplusplus
extern "C" {
#endif

struct gc_weak_table;

void gc_init();
void gc_exit();

void gc_enter();
void gc_leave(void *p, ...);

void *gc_malloc(size_t sz, void *parent, void (*finalizer)(void *));
void *gc_realloc(void *p, size_t sz, void *parent);
void gc_link(void *parent, void *prev, void *now);
void gc_collect();

void *gc_clone(void *from, size_t sz);

void gc_debug_print_nodes();

struct gc_weak_table *gc_weak_table(void *parent);
void *gc_weak_next(struct gc_weak_table *cont, int *iter);

#ifdef __cplusplus
}
#endif
