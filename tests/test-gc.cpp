#include "gc/gc.hpp"
#include <cstdio>

struct linked_list {
	struct linked_list *next;
	int data;
};

static void log_ptr(void *p)
{
	printf("free %p, data: %d\n", p, ((linked_list *) p)->data);
}

static struct linked_list *new_node(struct linked_list *parent)
{
	auto *ret = (struct linked_list *) gc_malloc(sizeof(struct linked_list), parent, log_ptr);
	printf("new %p\n", ret);

	if (parent != nullptr) {
		ret->next = parent->next;
		parent->next = ret;
	}
	else {
		ret->next = nullptr;
	}
	return ret;
}

int main()
{
	gc_init();
	gc_enter();

	linked_list *ll = new_node(nullptr);

	for (int i = 0; i < 3; ++i) {
		linked_list *item = new_node(ll);
		item->data = i;
	}

	gc_leave(nullptr);
	gc_exit();
	return 0;
}

