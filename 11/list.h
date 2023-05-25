#include <stdio.h>

#ifndef _LIST_H_
#define _LIST_H_

struct list {
    struct list* next;
    int value;
};

struct list* list_create(int);

void list_add_front(int, struct list**);

void list_add_back(int, struct list**);

int list_get(const struct list*, size_t);

void list_free(struct list*);

size_t list_length(const struct list*);

struct list* list_node_at(struct list*, size_t index);

int list_sum(const struct list*);

#endif