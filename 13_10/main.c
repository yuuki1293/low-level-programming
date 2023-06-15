#include <stdio.h>
#include <stdint.h>
#include "bmp.h"

#define FILENAME "picture.bmp"

int main() {
    rotate_bmp(FILENAME);

    return 0;
}