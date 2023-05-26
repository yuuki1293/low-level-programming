#include <stdio.h>
#include <stdbool.h>
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

LIST* load(const char* filename) {
    FILE* file;
    int value;
    file = fopen(filename, "rb");
    while (fread(&value, sizeof(int), 1, file)) {

    }

    return true;
}