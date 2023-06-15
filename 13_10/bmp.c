#include "bmp.h"

#define BM 0x4D42 /* BM */

#define SWAP(a, b) (a ^= b, b = a ^ b , a ^= b)

size_t load_bmp_header(FILE* file, struct bmp_header* header) {
    return fread(header, sizeof(struct bmp_header), 1, file);
}

void save_bmp(FILE* file, const struct bmp_header* header, const struct pixel* pixels) {
    fseek(file, 0, SEEK_SET);
    fwrite(header, sizeof(*header), 1, file);
    fwrite(pixels, sizeof(*pixels), header->biWidth * header->biHeight, file);
    fflush(file);
    return;
}

void rotate_bmp(char* file) {
    FILE* fp;
    struct bmp_header header;
    struct pixel* pixs;
    size_t h, w;

    fp = fopen(file, "rb");
    load_bmp_header(fp, &header);

    if (header.bfType != BM) {
        puts("Not a bmp file");
        return;
    }

    pixs = calloc(header.biHeight * header.biWidth, sizeof(struct pixel));

    for (h = 0;h < header.biHeight;h++) {
        for (w = 0; w < header.biWidth; w++) {
            if (fread(&pixs[header.biHeight * (w + 1) - 1 - h], sizeof(struct pixel), 1, fp) < 1) {
                puts("File is corrupt");
                return;
            }
        }
    }

    SWAP(header.biHeight, header.biWidth);

    fclose(fp);

    fp = fopen(file, "wb");
    save_bmp(fp, &header, pixs);
    fclose(fp);

    free(pixs);
    return;
}