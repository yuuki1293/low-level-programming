#include <stdio.h>

#ifndef _LIST_H_
#define _LIST_H_

struct list;

struct list *list_create(int);

void list_add_front(int, struct list **);

void list_add_back(int, struct list **);

int list_get(size_t, struct list *);

void list_free(struct list *);

size_t list_length(struct list *);

struct list *list_node_at(struct list *, size_t index);

int list_sum(struct list *);

#endif