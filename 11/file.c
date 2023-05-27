#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>
#include "list.h"
#include "file.h"

bool save(LIST* lst, const char* filename) {
    FILE* file;
    bool result;
    file = fopen(filename, "wb");
    while (lst) {
        fwrite(&(*lst).value, sizeof(int), 1, file);
        lst = (*lst).next;
    }
    result = fflush(file);
    fclose(file);

    return  result ? true : false;
}

bool load(LIST** lst, const char* filename) {
    FILE* file;
    int value;
    bool result = true;

    file = fopen(filename, "rb");

    while (fread(&value, sizeof(int), 1, file)) {
        *lst = list_create(value);
        lst = &((**lst).next);
    }

    if (ferror(file))
        result = false;

    fclose(file);

    return result;
}