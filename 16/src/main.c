#include <stdio.h>
#include <stdint.h>
#include <immintrin.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include "bmp.h"
#include "bench.h"

#define IMAGE_NAME "picture.bmp"
#define COUNT 128

void apply_sepia_filter(struct image* image);
void apply_sepia_filter_avx(struct image* image);
void apply_sepia_filter_haxe(struct image* image);
static void mul_matrix_sepia(struct pixel* const p);
static void mul_matrix_sepia_haxe(struct pixel* const p, struct pixel* new_p);
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

static void mul_matrix_sepia(struct pixel* const p) {
    static const __m128i upack_mask1 = { 0xFFFFFF00FFFFFF00, 0xFFFFFF00FFFFFF03 };
    static const __m128i upack_mask2 = { 0xFFFFFF00FFFFFF00, 0xFFFFFF03FFFFFF03 };
    static const __m128i upack_mask3 = { 0xFFFFFF00FFFFFF03, 0xFFFFFF03FFFFFF03 };
    static __m128 coe1 = { .131, .168, .189, .131 };
    static __m128 coe2 = { .534, .686, .769, .534 };
    static __m128 coe3 = { .272, .349, .393, .272 };
    uint8_t* vp = (void*)p;
    __m128 xm_b, xm_g, xm_r, bgr1, bgr2, bgr3; __m128i inprod;
    __m128i bgr1234 = _mm_loadu_si128((const __m128i*)vp);

    xm_b = _mm_cvtepi32_ps(_mm_shuffle_epi8(bgr1234, upack_mask1)); /* b1b1b1b2 */
    bgr1 = _mm_mul_ps(xm_b, coe1);
    bgr1234 = _mm_srli_si128(bgr1234, 1);
    xm_g = _mm_cvtepi32_ps(_mm_shuffle_epi8(bgr1234, upack_mask1));
    bgr2 = _mm_mul_ps(xm_g, coe2);
    bgr1234 = _mm_srli_si128(bgr1234, 1);
    xm_r = _mm_cvtepi32_ps(_mm_shuffle_epi8(bgr1234, upack_mask1));
    bgr3 = _mm_mul_ps(xm_r, coe3);
    bgr1234 = _mm_srli_si128(bgr1234, 1);
    inprod = _mm_cvttps_epi32(_mm_add_ps(_mm_add_ps(bgr1, bgr2), bgr3));
    *(int*)vp = _mm_cvtsi128_si32(_mm_packus_epi16(_mm_packs_epi32(inprod, inprod), inprod));
    vp += 4;
    coe1 = _mm_permute_ps(coe1, 0x79); /* 0x79 = 0b0111'1001*/
    coe2 = _mm_permute_ps(coe2, 0x79);
    coe3 = _mm_permute_ps(coe3, 0x79);

    xm_b = _mm_cvtepi32_ps(_mm_shuffle_epi8(bgr1234, upack_mask2)); /* b1b1b1b2 */
    bgr1 = _mm_mul_ps(xm_b, coe1);
    bgr1234 = _mm_srli_si128(bgr1234, 1);
    xm_g = _mm_cvtepi32_ps(_mm_shuffle_epi8(bgr1234, upack_mask2));
    bgr2 = _mm_mul_ps(xm_g, coe2);
    bgr1234 = _mm_srli_si128(bgr1234, 1);
    xm_r = _mm_cvtepi32_ps(_mm_shuffle_epi8(bgr1234, upack_mask2));
    bgr3 = _mm_mul_ps(xm_r, coe3);
    bgr1234 = _mm_srli_si128(bgr1234, 1);
    inprod = _mm_cvttps_epi32(_mm_add_ps(_mm_add_ps(bgr1, bgr2), bgr3));
    *(int*)vp = _mm_cvtsi128_si32(_mm_packus_epi16(_mm_packs_epi32(inprod, inprod), inprod));
    vp += 4;
    coe1 = _mm_permute_ps(coe1, 0x79); /* 0x79 = 0b0111'1001*/
    coe2 = _mm_permute_ps(coe2, 0x79);
    coe3 = _mm_permute_ps(coe3, 0x79);

    xm_b = _mm_cvtepi32_ps(_mm_shuffle_epi8(bgr1234, upack_mask3)); /* b1b1b1b2 */
    bgr1 = _mm_mul_ps(xm_b, coe1);
    bgr1234 = _mm_srli_si128(bgr1234, 1);
    xm_g = _mm_cvtepi32_ps(_mm_shuffle_epi8(bgr1234, upack_mask3));
    bgr2 = _mm_mul_ps(xm_g, coe2);
    bgr1234 = _mm_srli_si128(bgr1234, 1);
    xm_r = _mm_cvtepi32_ps(_mm_shuffle_epi8(bgr1234, upack_mask3));
    bgr3 = _mm_mul_ps(xm_r, coe3);
    bgr1234 = _mm_srli_si128(bgr1234, 1);
    inprod = _mm_cvttps_epi32(_mm_add_ps(_mm_add_ps(bgr1, bgr2), bgr3));
    *(int*)vp = _mm_cvtsi128_si32(_mm_packus_epi16(_mm_packs_epi32(inprod, inprod), inprod));
    vp += 4;
    coe1 = _mm_permute_ps(coe1, 0x79); /* 0x79 = 0b0111'1001*/
    coe2 = _mm_permute_ps(coe2, 0x79);
    coe3 = _mm_permute_ps(coe3, 0x79);
}

void apply_sepia_filter_haxe(struct image* image) {
    int rest = image->height * image->width;
    struct pixel* pixels = image->array;
    struct pixel* new_p = malloc(sizeof(struct pixel) * rest);
    int i = 0;

    while (rest >= 4) {
        mul_matrix_sepia_haxe(pixels + i, new_p + i);
        rest -= 4;
        i += 4;
    }

    free(image->array);
    image->array = new_p;

    while (rest > 0) {
        sepia_one(new_p + i);
        rest--;
        i++;
    }
}

/**
 * https://x.com/haxe/status/1791095731316748399
 */
static void mul_matrix_sepia_haxe(struct pixel* const p, struct pixel* const new_p) {
    static const __m128i code1 = { 0x0A07040109060300, 0xFFFFFFFF0B080502 };
    static const __m128i code2 = { 0x0602090501080400, 0xFFFFFFFF0B07030A };
    static const __m128 bias_bb = { .131f, .131f, .131f, .131f };
    static const __m128 bias_bg = { .534f, .534f, .534f, .534f };
    static const __m128 bias_br = { .272f, .272f, .272f, .272f };
    static const __m128 bias_gb = { .168f, .168f, .168f, .168f };
    static const __m128 bias_gg = { .686f, .686f, .686f, .686f };
    static const __m128 bias_gr = { .349f, .349f, .349f, .349f };
    static const __m128 bias_rb = { .189f, .189f, .189f, .189f };
    static const __m128 bias_rg = { .769f, .769f, .769f, .769f };
    static const __m128 bias_rr = { .393f, .393f, .393f, .393f };
    __m128 oldb, oldg, oldr, newb, newg, newr;
    __m128i bgr, bg, r_;

    bgr = _mm_shuffle_epi8(_mm_loadu_si128((const __m128i*)p), code1);
    oldb = _mm_cvtepi32_ps(_mm_cvtepu8_epi32(bgr));
    bgr = _mm_srli_si128(bgr, 4);
    oldg = _mm_cvtepi32_ps(_mm_cvtepu8_epi32(bgr));
    bgr = _mm_srli_si128(bgr, 4);
    oldr = _mm_cvtepi32_ps(_mm_cvtepu8_epi32(bgr));
    newb = _mm_fmadd_ps(oldr, bias_br, _mm_fmadd_ps(oldg, bias_bg, _mm_mul_ps(oldb, bias_bb)));
    newg = _mm_fmadd_ps(oldr, bias_gr, _mm_fmadd_ps(oldg, bias_gg, _mm_mul_ps(oldb, bias_gb)));
    bg = _mm_packus_epi32(_mm_cvttps_epi32(newb), _mm_cvttps_epi32(newg));
    newr = _mm_fmadd_ps(oldr, bias_rr, _mm_fmadd_ps(oldg, bias_rg, _mm_mul_ps(oldb, bias_rb)));
    r_ = _mm_packus_epi32(_mm_cvttps_epi32(newr), _mm_setzero_si128());
    bgr = _mm_shuffle_epi8(_mm_packus_epi16(bg, r_), code2);
    _mm_storeu_si128((__m128i*)new_p, bgr);
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
