#include <stdio.h>
#include "list.h"

#define N 3

int main(int argc, char **argv)
{
    int value;
    struct list *node;
    while (scanf("%d", &value) != EOF)
    {
        list_add_front(value, &node);
    }

    printf(
        "Sum is :%d\n",
        list_sum(node));

    printf(
        "%d element is :%d\n",
        N, list_get(N, node));

    list_free(node);
    return 0;
}