#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "higher_order.h"

void foreach(void (*f)(int), const struct list* list) {
    if (list) {
        f((*list).value);
        foreach(f, (*list).next);
    }
    else
        return;
}

struct list* map(int (*f)(int), const struct list* list) {
    struct list* new_list;
    if (list) {
        new_list = map(f, (*list).next);
        list_add_front(f((*list).value), &new_list);
        return new_list;
    }
    else
        return NULL;
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

struct list* iterate(int s, int n, int(*f)(int s)) {
    struct list* node;
    if (n) {
        node = malloc(sizeof(struct list));
        (*node).value = s;
        (*node).next = iterate(f(s), n - 1, f);
        return node;
    }
    else
        return NULL;
}