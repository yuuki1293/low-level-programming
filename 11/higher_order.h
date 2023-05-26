#ifndef _HIGHER_ORDER_H_
#define _HIGHER_ORDER_H_

#include "list.h"

void foreach(void (*f)(int), const LIST* list);

LIST* map(int (*f)(int), const LIST* list);

void map_mut(int (*f)(int), LIST* list);

int foldl(int acc, int(*f)(int x, int a), const LIST* list);

LIST* iterate(int s, int n, int(*f)(int s));

#endif