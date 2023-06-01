#include <stdio.h>
#include "mem.h"

int main() {
    int* a;
    heap_init(32);
    a = _malloc(sizeof(int));
    *a = 100;
    printf("%d\n", *a);

    return 0;
}