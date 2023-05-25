#include <malloc.h>
#include "list.h"

struct list* list_create(int num) {
    struct list node;
    struct list* p = malloc(sizeof(struct list));
    node.next = NULL;
    node.value = num;
    *p = node;
    return p;
}

void list_add_front(int num, struct list** pp) {
    struct list* p = list_create(num);
    (*p).next = *pp;
    *pp = p;
}

void list_add_back(int num, struct list** pp) {
    struct list* p = list_create(num);
    struct list* last = *pp;

    while ((*last).next != NULL) {
        last = (*last).next;
    }
    (*last).next = p;
}

int list_get(const struct list* p, size_t count) {
    size_t i;
    for (i = 0; i < count; i++) {
        p = (*p).next;
        if (p == NULL)
            return 0;
    }

    return (*p).value;
}

void list_free(struct list* p) {
    struct list* next = p;
    while (next != NULL) {
        free(next);
        next = (*next).next;
    }
}

size_t list_length(const struct list* p) {
    size_t count;
    while (p != NULL) {
        count++;
        p = (*p).next;
    }

    return count;
}

struct list* list_node_at(struct list* p, size_t index) {
    size_t i;
    for (i = 0; i < index; i++) {
        p = (*p).next;
        if (p == NULL)
            return NULL;
    }

    return p;
}

int list_sum(const struct list* p) {
    int sum = 0;
    while (p == NULL) {
        sum += (*p).value;
        p = (*p).next;
    }

    return sum;
}