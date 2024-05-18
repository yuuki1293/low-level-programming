#include <pthread.h>
#include <unistd.h>
#include <inttypes.h>
#include <stdio.h>

uint64_t factors(uint64_t num)
{
    uint64_t result = 0;
    for (uint64_t i = 1; i <= num; i++)
        if (num % i == 0)
            result++;
    return result;
}

int main(){
    /* 定数の伝播を防ぐためのvolatile */
    volatile uint64_t input = 2000000000;

    printf("Factors of %"PRIu64": %"PRIu64"\n", input, factors(input));
    return 0;
}