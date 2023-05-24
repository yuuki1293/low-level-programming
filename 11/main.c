#include <stdio.h>
#include "is_prime.h"
#include "list.h"
#include "scalar_product.h"
#include "higher_order.h"

void scan_list(struct list **list)
{
    int value;
    while (scanf("%d", &value) != EOF)
    {
        list_add_front(value, list);
    }
}

void print_int(int value)
{
    printf("%d ", value);
}

void printn_int(int value)
{
    printf("%d\n", value);
}

int main(int argc, char **argv)
{
    struct list *list = NULL;
    scan_list(&list);
    puts("");
    foreach (list, print_int)
        ;
    puts("");
    foreach (list, printn_int)
        ;
    

    return 0;
}