#include <stdio.h>
#include <limits.h>
#include "is_prime.h"
#include "list.h"
#include "scalar_product.h"
#include "higher_order.h"

void scan_list(struct list** list) {
    int value;
    while (scanf("%d", &value) != EOF) {
        list_add_front(value, list);
    }
}

void print_int(int value) {
    printf("%d ", value);
}

void printn_int(int value) {
    printf("%d\n", value);
}

int square(int x) {
    return x * x;
}

int cubic(int x) {
    return x * x * x;
}

int sum(int a, int b) {
    return a + b;
}

int max(int a, int b) {
    return a > b ? a : b;
}

int main() {
    struct list* list = NULL;
    scan_list(&list);
    puts("");
    foreach(print_int, list);
    puts("");
    foreach(printn_int, list);
    foreach(print_int, map(square, list));
    puts("");
    foreach(print_int, map(cubic, list));
    puts("");
    printf("sum: %d\n",
        foldl(0, sum, list));
    printf("max: %d\n",
        foldl(INT_MIN, max, list));

    return 0;
}