#include "bmp.h"
#include <file.h>

size_t load_bmp_header(FILE* file, struct bmp_header* header) {
    return fread(header, sizeof(struct bmp_header), 1, file);
}