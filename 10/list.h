#include <stdio.h>

#ifndef _LIST_H_
#define _LIST_H_

struct list;

struct list *list_create(int);

void list_add_front(int, struct list **);

void list_add_back(int, struct list **);

int list_get(size_t, struct list *);

void list_free(list);

int list_length(list);

int list_sum(list);

#endif