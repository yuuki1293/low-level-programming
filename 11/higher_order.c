#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "higher_order.h"

void foreach(void (*f)(int), const LIST* list) {
    if (list) {
        f((*list).value);
        foreach(f, (*list).next);
        return;
    }
    else
        return;
}

LIST* map(int (*f)(int), const LIST* list) {
    LIST* new_list;
    if (list) {
        new_list = map(f, (*list).next);
        list_add_front(f((*list).value), &new_list);
        return new_list;
    }
    else
        return NULL;
}

void map_mut(int (*f)(int), LIST* list) {
    if (list) {
        (*list).value = f((*list).value);
        map_mut(f, (*list).next);
    }
    else
        return;
}

int foldl(int acc, int(*f)(int x, int a), const LIST* list) {
    if (list)
        return foldl(f(acc, (*list).value), f, (*list).next);
    else
        return acc;
}

LIST* iterate(int s, int n, int(*f)(int s)) {
    LIST* node;
    if (n) {
        node = malloc(sizeof(LIST));
        (*node).value = s;
        (*node).next = iterate(f(s), n - 1, f);
        return node;
    }
    else
        return NULL;
}