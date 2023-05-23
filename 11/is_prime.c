#include <stdio.h>

int is_prime(unsigned long n)
{
    size_t i;
    for (i = 2; i < n; i++)
    {
        if (n % i == 0)
            return 0;
    }
    return 1;
}