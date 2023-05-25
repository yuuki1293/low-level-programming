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
    foreach(list, print_int);
    puts("");
    foreach(list, printn_int);
    foreach(map(list, square), print_int);
    puts("");
    foreach(map(list, cubic), print_int);
    puts("");
    printf("sum: %d\n",
        foldl(0, sum, list));
    printf("max: %d\n",
        foldl(INT_MIN, max, list));

    return 0;
}