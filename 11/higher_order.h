#ifndef _HIGHER_ORDER_H_
#define _HIGHER_ORDER_H_

void foreach (struct list *, void (*)(int));

struct list *map(const struct list *, int (*)(int));

#endif