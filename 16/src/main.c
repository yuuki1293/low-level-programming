#include <stdio.h>
#include <stdint.h>
#include <immintrin.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include "bmp.h"

void apply_sepia_filter(struct image* image);
void apply_sepia_filter_avx(struct image* image);
void apply_sepia_filter_haxe(struct image* image);
static void mul_matrix_sepia(struct pixel* const p);
static void mul_matrix_sepia_haxe(struct pixel* const p);
static unsigned char sat(uint64_t x);
static void sepia_one(struct pixel* const pixel);

int main() {
    struct rusage r;
    struct timeval start;
    struct timeval end;
    long res;
    struct bmp_header header;
    struct image image;
    int i;
    FILE* fp;

    fp = fopen("log.csv", "w");
    for (i = 0; i < 128; i++)
    {
        load_bmp("picture.bmp", &header, &image);
        getrusage(RUSAGE_SELF, &r);
        start = r.ru_utime;

        apply_sepia_filter(&image);

        getrusage(RUSAGE_SELF, &r);
        end = r.ru_utime;
        res = ((end.tv_sec - start.tv_sec) * 1000000L) + end.tv_usec - start.tv_usec;
        fprintf(fp, "%ld\n", res);
        save_bmp("sepia_picture.bmp", &header, &image);
    }
    fclose(fp);

    fp = fopen("log_avx.csv", "w");
    for (i = 0; i < 128; i++)
    {
        load_bmp("picture.bmp", &header, &image);
        getrusage(RUSAGE_SELF, &r);
        start = r.ru_utime;

        apply_sepia_filter_avx(&image);

        getrusage(RUSAGE_SELF, &r);
        end = r.ru_utime;
        res = ((end.tv_sec - start.tv_sec) * 1000000L) + end.tv_usec - start.tv_usec;
        fprintf(fp, "%ld\n", res);
        save_bmp("sepia_picture_avx.bmp", &header, &image);
    }
    fclose(fp);

    return 0;
}

void apply_sepia_filter(struct image* image) {
    int rest = image->height * image->width;
    struct pixel* pixels = image->array;
    int i = 0;

    while (rest > 0) {
        sepia_one(pixels + i);
        rest--;
        i++;
    }
}

void apply_sepia_filter_avx(struct image* image) {
    int rest = image->height * image->width;
    struct pixel* pixels = image->array;
    int i = 0;

    while (rest >= 4) {
        mul_matrix_sepia(pixels + i);
        rest -= 4;
        i += 4;
    }
    while (rest > 0) {
        sepia_one(pixels + i);
        rest--;
        i++;
    }
}

static void mul_matrix_sepia(struct pixel* const p) {
#define c11 .131f
#define c12 .534f
#define c13 .272f
#define c21 .168f
#define c22 .686f
#define c23 .349f
#define c31 .189f
#define c32 .769f
#define c33 .393f

    uint8_t p_vec[3][3][4] = { 0 };
    int i, j;
    uint8_t mask_arr[16] __attribute__((aligned(16))) = { 0x00, 0x04, 0x08, 0x0C };
    __m128i xm_shuf_mask = _mm_load_si128((__m128i*)mask_arr);

    static const float coefficient[3][3][4] = {
        {{c11, c21, c31, c11},
        {c12, c22, c32, c12},
        {c13, c23, c33, c13}},
        {{c21, c31, c11, c21},
        {c22, c32, c12, c22},
        {c23, c33, c13, c23}},
        {{c31, c11, c21, c31},
        {c32, c12, c22, c32},
        {c33, c13, c23, c33}},
    };

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            p_vec[0][i][j] = p[i * j / 3].b;
            p_vec[1][i][j] = p[i * j / 3].g;
            p_vec[2][i][j] = p[i * j / 3].r;
        }
    }

    for (i = 0; i < 3; i++) {
        __m128 xm_b, xm_g, xm_r; __m128i xm_inprod;

        xm_b = _mm_cvtepi32_ps(_mm_cvtepu8_epi32(_mm_loadu_si128((__m128i*)p_vec[0][i])));
        xm_g = _mm_cvtepi32_ps(_mm_cvtepu8_epi32(_mm_loadu_si128((__m128i*)p_vec[1][i])));
        xm_r = _mm_cvtepi32_ps(_mm_cvtepu8_epi32(_mm_loadu_si128((__m128i*)p_vec[2][i])));

        xm_b = _mm_mul_ps(xm_b, _mm_load_ps(coefficient[i][0]));
        xm_g = _mm_mul_ps(xm_g, _mm_load_ps(coefficient[i][1]));
        xm_r = _mm_mul_ps(xm_r, _mm_load_ps(coefficient[i][2]));

        xm_inprod = _mm_cvttps_epi32(_mm_add_ps(_mm_add_ps(xm_b, xm_g), xm_r));

        *(int*)((uint8_t*)p + 4 * i) = 
            _mm_cvtsi128_si32(_mm_shuffle_epi8(xm_inprod, xm_shuf_mask));
    }
}

void apply_sepia_filter_haxe(struct image* image) {
    int rest = image->height * image->width;
    struct pixel* pixels = image->array;
    int i = 0;

    while (rest >= 4) {
        mul_matrix_sepia_haxe(pixels + i);
        rest -= 4;
        i += 4;
    }
    while (rest > 0) {
        sepia_one(pixels + i);
        rest--;
        i++;
    }
}

/**
 * https://x.com/haxe/status/1790881079358075255
 */
static void mul_matrix_sepia_haxe(struct pixel* const p) {
    __m128 bgr1, bgr2, bgr3, bgr4, b1, g1, r1, b2, g2, r2, b3, g3, r3, b4, g4, r4;
    __m128i bgr12, bgr34;
    static const __m128 bias_b = { .131, .534, .272};
    static const __m128 bias_g = { .168, .686, .349};
    static const __m128 bias_r = { .189, .769, .393};

    __m128i bgr1234 = _mm_loadu_si128((const __m128i*)p);
    bgr1 = _mm_cvtepi32_ps(_mm_cvtepu8_epi32(bgr1234));
    bgr1234 = _mm_srli_si128(bgr1234, 4);
    bgr2 = _mm_cvtepi32_ps(_mm_cvtepu8_epi32(bgr1234));
    bgr1234 = _mm_srli_si128(bgr1234, 4);
    bgr3 = _mm_cvtepi32_ps(_mm_cvtepu8_epi32(bgr1234));
    bgr1234 = _mm_srli_si128(bgr1234, 4);
    bgr4 = _mm_cvtepi32_ps(_mm_cvtepu8_epi32(bgr1234));

    b1 = _mm_dp_ps(bgr1, bias_b, 0b0111'0001);
    g1 = _mm_dp_ps(bgr1, bias_g, 0b0111'0001);
    r1 = _mm_dp_ps(bgr1, bias_r, 0b0111'0001);
    bgr1 = _mm_unpacklo_ps(_mm_unpacklo_ps(b1, r1), g1);
    b2 = _mm_dp_ps(bgr2, bias_b, 0b0111'0001);
    g2 = _mm_dp_ps(bgr2, bias_g, 0b0111'0001);
    r2 = _mm_dp_ps(bgr2, bias_r, 0b0111'0001);
    bgr2 = _mm_unpacklo_ps(_mm_unpacklo_ps(b2, r2), g2);
    bgr12 = _mm_packus_epi32(_mm_cvttps_epi32(bgr1), _mm_cvttps_epi32(bgr2));
    b3 = _mm_dp_ps(bgr3, bias_b, 0b0111'0001);
    g3 = _mm_dp_ps(bgr3, bias_g, 0b0111'0001);
    r3 = _mm_dp_ps(bgr3, bias_r, 0b0111'0001);
    bgr3 = _mm_unpacklo_ps(_mm_unpacklo_ps(b3, r3), g3);
    b4 = _mm_dp_ps(bgr4, bias_b, 0b0111'0001);
    g4 = _mm_dp_ps(bgr4, bias_g, 0b0111'0001);
    r4 = _mm_dp_ps(bgr4, bias_r, 0b0111'0001);
    bgr4 = _mm_unpacklo_ps(_mm_unpacklo_ps(b4, r4), g4);
    bgr34 = _mm_packus_epi32(_mm_cvttps_epi32(bgr3), _mm_cvttps_epi32(bgr4));

    _mm_storeu_si128((__m128i*)p, _mm_packus_epi16(bgr12, bgr34));
}

static unsigned char sat(uint64_t x) {
    if (x < 256)
        return x;
    return 255;
}

static void sepia_one(struct pixel* const pixel) {
    static const float c[3][3] = {
        { .393f, .769f, .189f },
        { .349f, .686f, .168f },
        { .272f, .534f, .131f } };
    struct pixel const old = *pixel;

    pixel->r = sat(
        old.r * c[0][0] + old.g * c[0][1] + old.b * c[0][2]
    );
    pixel->g = sat(
        old.r * c[1][0] + old.g * c[1][1] + old.b * c[1][2]
    );
    pixel->b = sat(
        old.r * c[2][0] + old.g * c[2][1] + old.b * c[2][2]
    );
}
