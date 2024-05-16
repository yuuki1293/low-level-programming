#ifndef _BENCH_H_
#define _BENCH_H_

#include "bmp.h"

void bench(void (*target)(struct image*), const char* image_name ,const char* const label, int count);

#endif