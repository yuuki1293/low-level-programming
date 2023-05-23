#include <malloc.h>
#include "list.h"

struct list
{
    struct list *next;
    int value;
};

struct list *list_create(int num)
{
    struct list node;
    node.next = NULL;
    node.value = num;
    struct list *p = malloc(sizeof(struct list));
    *p = node;
    return p;
}

void list_add_front(int num, struct list **pp)
{
    struct list *p = list_create(num);
    (*p).next = *pp;
    *pp = p;
}

void list_add_back(int num, struct list **pp)
{
    struct list *p = list_create(num);
    (**pp).next = p;
}