#include <stdio.h>
#include <stdbool.h>
#include "list.h"
#include "file.h"

bool save(LIST* lst, const char* filename) {
    FILE* file;
    file = fopen(filename, "w");
    while (lst) {
        fwrite(&(*lst).value, sizeof(int), 1, file);
        lst = (*lst).next;
    }

    return fflush(file) ? true : false;
}