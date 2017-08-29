#include "gc/pure_gc.hpp"
#include <cstdio>

using namespace dvm::gc::pure_gc;

struct linked_list {
    struct linked_list *next;
    int data;
};

static void log_ptr(void *p) {
    printf("free %p, data: %d\n", p, ((linked_list *) p)->data);
}

static void log_common_ptr(void *p) {
    printf("free %p\n", p);
}

static struct linked_list *new_node(gc_context *gc, struct linked_list *parent) {
    auto *ret = (struct linked_list *) gc->malloc(sizeof(struct linked_list), parent, log_ptr);
    printf("new %p\n", ret);

    if (parent != nullptr) {
        ret->next = parent->next;
        parent->next = ret;
    } else {
        ret->next = nullptr;
    }
    return ret;
}

int main() {
    gc_context gc;
    gc.enter();

    linked_list *ll = new_node(&gc, nullptr);

    for (int i = 0; i < 3; ++i) {
        linked_list *item = new_node(&gc, ll);
        item->data = i;
    }
    gc.leave(nullptr);
    gc.collect();

    gc_context gc2;
    int *p = (int *) gc2.malloc(sizeof(int), nullptr, log_common_ptr);
    gc2.collect();
    return 0;
}
