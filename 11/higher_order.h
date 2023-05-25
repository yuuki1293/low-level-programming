#ifndef _HIGHER_ORDER_H_
#define _HIGHER_ORDER_H_

#include "list.h"

void foreach(void (*f)(int), const struct list* list);

struct list* map(int (*f)(int), const struct list* list);

void map_mut(int (*f)(int), struct list* list);

int foldl(int acc, int(*f)(int x, int a), const struct list* list);

struct list* iterate(int s, int n, int(*f)(int s));

#endif