#include "plasma.h"
#include "boxes.h"

void demo_init(void) {
    plasma_init();
    boxes_init();
}

void demo_frame(uint32_t *pixels, uint32_t time) {
    if (time % 8000 < 4000) {
        plasma_frame(pixels, time);
    } else {
        boxes_frame(pixels, time);
    }
}
