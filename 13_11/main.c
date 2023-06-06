#include <stdio.h>
#include "mem.h"

int main() {
    int* a;
    long* b;
    heap_init(32);
    a = _malloc(sizeof(int));
    b = _malloc(sizeof(long));
    *a = 100;
    *b = 200;
    *b += *a;
    _free(a);
    _free(b);
    printf("%ld\n", *b);

    return 0;
}