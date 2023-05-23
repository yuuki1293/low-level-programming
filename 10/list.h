#include <stdio.h>

#ifndef _LIST_C_
#define _LIST_C_

struct node;

typedef struct node *list;

list list_create(int);

void list_add_front(int, list *);

void list_add_back(int, list *);

int list_get(size_t, const list);

void list_free(list);

int list_length(list);

int list_sum(list);

#endif