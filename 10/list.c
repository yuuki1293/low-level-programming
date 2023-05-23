#include "list.h"

struct list
{
    struct list *next;
    int value;
};

struct list list_create(int num)
{
    struct list node;
    node.next = NULL;
    node.value = num;
    return node;
}