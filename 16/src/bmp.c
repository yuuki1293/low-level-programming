#include "bmp.h"

#define BM 0x4D42 /* (uint16_t)(*"BM") */

#define SWAP(a, b) (a ^= b, b = a ^ b , a ^= b)

size_t load_bmp(const char* filename, struct bmp_header* header, struct image* image) {
    FILE* fp = fopen(filename, "rb");
    if (fread(header, sizeof(struct bmp_header), 1, fp) < 1) {
        fclose(fp);
        return 1;
    }

    image->height = header->biHeight;
    image->width = header->biWidth;
    image->array = calloc(header->biHeight * header->biWidth, sizeof(struct pixel));

    if (fread(image->array, sizeof(struct pixel), image->height * image->width, fp) < 1) {
        fclose(fp);
        return 1;
    }
    return fclose(fp);
}

size_t save_bmp(const char* filename, struct bmp_header* header, struct image* image) {
    FILE* fp = fopen(filename, "wb");
    if (!fwrite(header, sizeof(struct bmp_header), 1, fp)) {
        fclose(fp);
        return 1;
    }
    if (!fwrite(image->array, sizeof(struct pixel), header->biWidth * header->biHeight, fp)) {
        fclose(fp);
        return 1;
    }
    return fclose(fp);
}