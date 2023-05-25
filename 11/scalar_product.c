#include <stdio.h>
#include "scalar_product.h"

int scalar_product(const int* a, const int* b, const size_t count) {
    size_t i;
    int scalar = 0;
    for (i = 0; i < count; i++)
        scalar += a[i] * b[i];
    return scalar;
}