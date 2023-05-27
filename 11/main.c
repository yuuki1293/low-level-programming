#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include "list.h"
#include "higher_order.h"
#include "file.h"

#define FILENAME_TXT "hoge.txt"
#define FILENAME_BIN "hoge.bin"

void scan_list(LIST** list) {
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

int abs(int a) {
    return a > 0 ? a : -a;
}

int double_int(int a) {
    return a * 2;
}

int main() {
    LIST* list = NULL;

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
    map_mut(abs, list);
    foreach(print_int, list);
    puts("");
    list = NULL;

    list = iterate(1, 10, double_int);
    foreach(print_int, list);
    puts("");

    save(list, FILENAME_TXT);
    list = NULL;
    load(&list, FILENAME_TXT);
    foreach(print_int, list);
    puts("");

    serialize(list, FILENAME_BIN);
    list = NULL;
    deserialize(&list, FILENAME_BIN);
    foreach(print_int, list);
    puts("");

    return 0;
}