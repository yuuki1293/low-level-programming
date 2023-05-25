#include <stdio.h>
#include "list.h"
#include "higher_order.h"

void foreach(const struct list* list, void (*f)(int)) {
    while (list != NULL) {
        f((*list).value);
        list = (*list).next;
    }

    return;
}

struct list* map(const struct list* list, int (*f)(int)) {
    struct list* new_list;
    struct list* new_node;

    if (!list)
        return NULL;

    new_list = list_create(f((*list).value));
    new_node = new_list;
    list = (*list).next;
    while (list != NULL) {
        (*new_node).next = list_create(f((*list).value));
        new_node = (*new_node).next;
        list = (*list).next;
    }
    return new_list;
}

int foldl(int acc, int(*f)(int x, int a), const struct list* list) {
    if (list)
        return foldl(f(acc, (*list).value), f, (*list).next);
    else
        return acc;
}