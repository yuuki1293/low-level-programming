#include <stdio.h>
#include "list.h"

void foreach(struct list* list, void (*f)(int)){
    while (list != NULL)
    {
        f((*list).value);
        list = (*list).next;
    }
}