#include <stdio.h>

int vector1[] = {1, 2, 3, 4, 5};
int vector2[] = {2, 4, 7, 1, 7};

int scalar_product(const int *a, const int *b, const size_t count)
{
    size_t i;
    int scalar = 0;
    for (i = 0; i < count; i++)
        scalar += a[i] * b[i];
    return scalar;
}

int main(int argc, char **argv)
{
    printf(
        "The scalar product is: %d\n",
        scalar_product(vector1, vector2, sizeof(vector1) / sizeof(int)));
    return 0;
}