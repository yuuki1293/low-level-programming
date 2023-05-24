#include <stdio.h>
#include "list.h"

void foreach (struct list *list, void (*f)(int))
{
    while (list != NULL)
    {
        f((*list).value);
        list = (*list).next;
    }
}

struct list *map(const struct list *list, int (*f)(int))
{
    struct list *new_list;
    struct list *new_node;

    if (!list)
        return NULL;

    new_list = list_create(f((*list).value));
    new_node = new_list;
    list = (*list).next;
    while (list != NULL)
    {
        (*new_node).next = list_create(f((*list).value));
        list = (*list).next;
    }
    return new_list;
}