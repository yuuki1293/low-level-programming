#include <malloc.h>
#include "list.h"

LIST* list_create(int num) {
    LIST node;
    LIST* p = malloc(sizeof(LIST));
    node.next = NULL;
    node.value = num;
    *p = node;
    return p;
}

void list_add_front(int num, LIST** pp) {
    LIST* p = list_create(num);
    (*p).next = *pp;
    *pp = p;
}

void list_add_back(int num, LIST** pp) {
    LIST* p = list_create(num);
    LIST* last = *pp;

    while ((*last).next != NULL) {
        last = (*last).next;
    }
    (*last).next = p;
}

int list_get(const LIST* p, size_t count) {
    size_t i;
    for (i = 0; i < count; i++) {
        p = (*p).next;
        if (p == NULL)
            return 0;
    }

    return (*p).value;
}

void list_free(LIST* p) {
    LIST* next = p;
    while (next != NULL) {
        free(next);
        next = (*next).next;
    }
}

size_t list_length(const LIST* p) {
    size_t count;
    while (p != NULL) {
        count++;
        p = (*p).next;
    }

    return count;
}

LIST* list_node_at(LIST* p, size_t index) {
    size_t i;
    for (i = 0; i < index; i++) {
        p = (*p).next;
        if (p == NULL)
            return NULL;
    }

    return p;
}

int list_sum(const LIST* p) {
    int sum = 0;
    while (p == NULL) {
        sum += (*p).value;
        p = (*p).next;
    }

    return sum;
}