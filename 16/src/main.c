#include <stdio.h>
#include <stdint.h>
#include <immintrin.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include "bmp.h"
#include "bench.h"

#define IMAGE_NAME "picture.bmp"
#define COUNT 1

void apply_sepia_filter(struct image* image);
void apply_sepia_filter_avx(struct image* image);
void apply_sepia_filter_haxe(struct image* image);
static void mul_matrix_sepia(struct pixel* const p);
static void mul_matrix_sepia_haxe(struct pixel* const p);
static unsigned char sat(uint64_t x);
static void sepia_one(struct pixel* const pixel);

int main() {
    bench(&apply_sepia_filter, IMAGE_NAME, "general", COUNT);
    bench(&apply_sepia_filter_avx, IMAGE_NAME, "avx", COUNT);
    bench(&apply_sepia_filter_haxe, IMAGE_NAME, "haxe", COUNT);

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

static void mul_matrix_sepia(struct pixel* p) {
    static const __m128 bias_b = { .131, .534, .272, .131 };
    static const __m128 bias_g = { .168, .686, .349, .168 };
    static const __m128 bias_r = { .189, .769, .393, .189 };
    static const __m128i pack_mask = { 0x0004080C00000000 };
    static const __m128i upack_mask1 = { 0x00FFFFFF00FFFFFF, 0x00FFFFFF03FFFFFF};
    static const __m128i upack_mask2 = { 0x00FFFFFF00FFFFFF, 0x03FFFFFF03FFFFFF};
    static const __m128i upack_mask3 = { 0x00FFFFFF03FFFFFF, 0x03FFFFFF03FFFFFF};
    __m128 xm_b, xm_g, xm_r, bgr1, bgr2, bgr3; __m128i inprod;
    __m128i bgr1234 = _mm_loadu_si128((const __m128i*)p);

    xm_b = _mm_cvtepi32_ps(_mm_shuffle_epi8(bgr1234, upack_mask1)); /* b1b1b1b2 */
    bgr1 = _mm_mul_ps(xm_b, bias_b);
    bgr1234 = _mm_srli_si128(bgr1234, 1);
    xm_g = _mm_cvtepi32_ps(_mm_shuffle_epi8(bgr1234, upack_mask1));
    bgr2 = _mm_mul_ps(xm_g, bias_g);
    bgr1234 = _mm_srli_si128(bgr1234, 1);
    xm_r = _mm_cvtepi32_ps(_mm_shuffle_epi8(bgr1234, upack_mask1));
    bgr3 = _mm_mul_ps(xm_r, bias_r);
    bgr1234 = _mm_srli_si128(bgr1234, 1);
    inprod = _mm_cvttps_epi32(_mm_add_ps(_mm_add_ps(bgr1, bgr2), bgr3));
    *(int*)((uint8_t*)p) =_mm_cvtsi128_si32(_mm_shuffle_epi8(inprod, pack_mask));
    (int*)p++;
    _mm_permute_ps(bias_b, 0x79); /* 0x79 = 0b0111'1001*/
    _mm_permute_ps(bias_g, 0x79);
    _mm_permute_ps(bias_r, 0x79);

    xm_b = _mm_cvtepi32_ps(_mm_shuffle_epi8(bgr1234, upack_mask2)); /* b1b1b1b2 */
    bgr1 = _mm_mul_ps(xm_b, bias_b);
    bgr1234 = _mm_srli_si128(bgr1234, 1);
    xm_g = _mm_cvtepi32_ps(_mm_shuffle_epi8(bgr1234, upack_mask2));
    bgr2 = _mm_mul_ps(xm_g, bias_g);
    bgr1234 = _mm_srli_si128(bgr1234, 1);
    xm_r = _mm_cvtepi32_ps(_mm_shuffle_epi8(bgr1234, upack_mask2));
    bgr3 = _mm_mul_ps(xm_r, bias_r);
    bgr1234 = _mm_srli_si128(bgr1234, 1);
    inprod = _mm_cvttps_epi32(_mm_add_ps(_mm_add_ps(bgr1, bgr2), bgr3));
    *(int*)((uint8_t*)p) =_mm_cvtsi128_si32(_mm_shuffle_epi8(inprod, pack_mask));
    (int*)p++;
    _mm_permute_ps(bias_b, 0x79); /* 0x79 = 0b0111'1001*/
    _mm_permute_ps(bias_g, 0x79);
    _mm_permute_ps(bias_r, 0x79);

    xm_b = _mm_cvtepi32_ps(_mm_shuffle_epi8(bgr1234, upack_mask3)); /* b1b1b1b2 */
    bgr1 = _mm_mul_ps(xm_b, bias_b);
    bgr1234 = _mm_srli_si128(bgr1234, 1);
    xm_g = _mm_cvtepi32_ps(_mm_shuffle_epi8(bgr1234, upack_mask3));
    bgr2 = _mm_mul_ps(xm_g, bias_g);
    bgr1234 = _mm_srli_si128(bgr1234, 1);
    xm_r = _mm_cvtepi32_ps(_mm_shuffle_epi8(bgr1234, upack_mask3));
    bgr3 = _mm_mul_ps(xm_r, bias_r);
    bgr1234 = _mm_srli_si128(bgr1234, 1);
    inprod = _mm_cvttps_epi32(_mm_add_ps(_mm_add_ps(bgr1, bgr2), bgr3));
    *(int*)((uint8_t*)p) =_mm_cvtsi128_si32(_mm_shuffle_epi8(inprod, pack_mask));
    (int*)p++;
    _mm_permute_ps(bias_b, 0x79); /* 0x79 = 0b0111'1001*/
    _mm_permute_ps(bias_g, 0x79);
    _mm_permute_ps(bias_r, 0x79);
}

void apply_sepia_filter_haxe(struct image* image) {
    int rest = image->height * image->width;
    struct pixel* pixels = image->array;
    int i = 0;

    while (rest >= 16) {
        mul_matrix_sepia_haxe(pixels + i);
        rest -= 16;
        i += 16;
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
    static const __m128 bias_b = { .131, .534, .272 };
    static const __m128 bias_g = { .168, .686, .349 };
    static const __m128 bias_r = { .189, .769, .393 };

    __m128i bgr1234 = _mm_loadu_si128((const __m128i*)p); /* bgr1234 = r4g4b4r3g3b3r2g2b2r1g1b1 */
    bgr1 = _mm_cvtepi32_ps(_mm_cvtepu8_epi32(bgr1234));
    bgr1234 = _mm_srli_si128(bgr1234, 4);
    bgr2 = _mm_cvtepi32_ps(_mm_cvtepu8_epi32(bgr1234));
    bgr1234 = _mm_srli_si128(bgr1234, 4);
    bgr3 = _mm_cvtepi32_ps(_mm_cvtepu8_epi32(bgr1234));
    bgr1234 = _mm_srli_si128(bgr1234, 4);
    bgr4 = _mm_cvtepi32_ps(_mm_cvtepu8_epi32(bgr1234));

    b1 = _mm_dp_ps(bgr1, bias_b, 0x71); /* 0x71 = 0b0111'0001*/
    g1 = _mm_dp_ps(bgr1, bias_g, 0x71);
    r1 = _mm_dp_ps(bgr1, bias_r, 0x71);
    bgr1 = _mm_unpacklo_ps(_mm_unpacklo_ps(b1, r1), g1);
    b2 = _mm_dp_ps(bgr2, bias_b, 0x71);
    g2 = _mm_dp_ps(bgr2, bias_g, 0x71);
    r2 = _mm_dp_ps(bgr2, bias_r, 0x71);
    bgr2 = _mm_unpacklo_ps(_mm_unpacklo_ps(b2, r2), g2);
    bgr12 = _mm_packus_epi32(_mm_cvttps_epi32(bgr1), _mm_cvttps_epi32(bgr2));
    b3 = _mm_dp_ps(bgr3, bias_b, 0x71);
    g3 = _mm_dp_ps(bgr3, bias_g, 0x71);
    r3 = _mm_dp_ps(bgr3, bias_r, 0x71);
    bgr3 = _mm_unpacklo_ps(_mm_unpacklo_ps(b3, r3), g3);
    b4 = _mm_dp_ps(bgr4, bias_b, 0x71);
    g4 = _mm_dp_ps(bgr4, bias_g, 0x71);
    r4 = _mm_dp_ps(bgr4, bias_r, 0x71);
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
