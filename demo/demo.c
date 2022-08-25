#include "plasma.h"
#include "boxes.h"
#include "rotozoom.h"
#include "crosstunnel.h"

void demo_init(void) {
    crosstunnel_init();
    plasma_init();
    rotozoom_init();
    boxes_init();
}

void demo_frame(uint32_t *pixels, uint32_t time) {
    uint32_t scene = (time / 5486) % 4;
    switch (scene) {
        case 0:
            crosstunnel_frame(pixels, time);
            break;
        case 1:
            rotozoom_frame(pixels, time);
            break;
        case 2:
            boxes_frame(pixels, time);
            break;
        default:
            plasma_frame(pixels, time);
    }
}
