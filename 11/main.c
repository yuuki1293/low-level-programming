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

int main(int argc, char **argv)
{
    struct list* list;
    scan_list(&list);
    foreach(list, puts);

    return 0;
}