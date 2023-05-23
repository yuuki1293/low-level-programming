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

int main(int argc, char **argv)
{
    unsigned long n;
    scanf("%lu", &n);
    printf(
        "is prime: %d\n",
        is_prime(n));
    return 0;
}