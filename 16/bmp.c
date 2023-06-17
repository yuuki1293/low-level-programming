#include "bmp.h"

#define BM 0x4D42 /* (uint16_t)(*"BM") */

#define SWAP(a, b) (a ^= b, b = a ^ b , a ^= b)

size_t load_bmp_header(FILE* file, struct bmp_header* header) {
    return fread(header, sizeof(struct bmp_header), 1, file);
}

void save_bmp(FILE* file, const struct bmp_header* header, const struct image* image) {
    fwrite(header, sizeof(*header), 1, file);
    fwrite(image->array, sizeof(struct pixel), header->biWidth * header->biHeight, file);
    return;
}