#ifndef _HIGHER_ORDER_H_
#define _HIGHER_ORDER_H_

#include "list.h"

void foreach(const struct list*, void (*)(int));

struct list* map(const struct list*, int (*)(int));

int foldl(int acc, int(*f)(int x, int a), const struct list* list);

#endif