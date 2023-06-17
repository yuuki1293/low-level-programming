#ifndef _BMP_H_
#define _BMP_H_

#include <stdint.h>
#include <stdio.h>
#include <malloc.h>

struct __attribute__((packed))
    bmp_header {
    uint16_t bfType;
    uint32_t bfileSize;
    uint32_t bfReserved;
    uint32_t bfOffBits;
    uint32_t biSize;
    uint32_t biWidth;
    uint32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    uint32_t biXPelsPerMeter;
    uint32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};

struct pixel {
    unsigned char b, g, r;
};

struct image {
    uint32_t width, height;
    struct pixel* array;
};

/**
 * @brief ファイルポインタからbmp_headerを取り出す。ファイルポインタはヘッダサイズだけ進む。
 * 
 * @param file ファイルポインタ。
 * @param header bmp_headerのポインタ。
 * @return size_t 読み込んだデータ数。0か1。
 */
size_t load_bmp_header(FILE* file, struct bmp_header* header);

void save_bmp(FILE* file, const struct bmp_header* header, const struct image* image);

#endif /*_BMP_H_*/