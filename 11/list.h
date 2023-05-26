#include <stdio.h>

#ifndef _LIST_H_
#define _LIST_H_

typedef struct list {
    struct list* next;
    int value;
} LIST;

LIST* list_create(int);

void list_add_front(int, LIST**);

void list_add_back(int, LIST**);

int list_get(const LIST*, size_t);

void list_free(LIST*);

size_t list_length(const LIST*);

LIST* list_node_at(LIST*, size_t index);

int list_sum(const LIST*);

#endif