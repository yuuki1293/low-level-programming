#include <stdio.h>
#include "list.h"
#include "higher_order.h"

void foreach(void (*f)(int), const struct list* list) {
    while (list != NULL) {
        f((*list).value);
        list = (*list).next;
    }

    return;
}

struct list* map(int (*f)(int), const struct list* list) {
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

void map_mut(int (*f)(int), struct list* list) {
    if (list) {
        (*list).value = f((*list).value);
        map_mut(f, (*list).next);
    }
    else
        return;
}

int foldl(int acc, int(*f)(int x, int a), const struct list* list) {
    if (list)
        return foldl(f(acc, (*list).value), f, (*list).next);
    else
        return acc;
}