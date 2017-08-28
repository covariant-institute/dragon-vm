#include "gc/gc.hpp"
#include <cstdio>

struct test {
    struct test *next;
};

static void log_ptr(void *p) {
    printf("free %p\n", p);
}

static struct test *new_test(struct test *parent) {
    struct test *ret = (struct test *) gc_malloc(sizeof(struct test), parent, log_ptr);
    printf("new %p\n", ret);
    if (parent) {
        ret->next = parent->next;
        parent->next = ret;
    } else {
        ret->next = 0;
    }
    return ret;
}

static void *test(struct gc_weak_table *weak) {
    struct test *p;
    int i;

    gc_enter();

    gc_enter();
    for (i = 0; i < 4; i++) {
        p = new_test(0);
        gc_link(weak, 0, p);
    }

    /* after gc_leave , only last p leave in the stack */
    gc_leave(p, 0);

    /* p will not be collected */
    gc_collect();

    p->next = new_test(p);

    /* one node can be linked to parent more than once */
    gc_link(p, 0, p->next);

    gc_debug_print_nodes();

    gc_link(p, p->next, 0);

    gc_debug_print_nodes();

    /* p will not be exist on the stack after gc_leave , it can be collected. */
    gc_leave(p->next, 0);

    gc_link(weak, 0, p->next);

    return p->next;
}

int main() {
    gc_init();

    test(gc_weak_table(nullptr));

    gc_exit();
    return 0;
}

