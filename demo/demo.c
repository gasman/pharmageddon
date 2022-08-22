#include "plasma.h"

void demo_init(void) {
    plasma_init();
}

void demo_frame(uint32_t *pixels, uint32_t time) {
    plasma_frame(pixels, time);
}
