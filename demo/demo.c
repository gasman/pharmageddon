#include "plasma.h"
#include "boxes.h"
#include "rotozoom.h"
#include "crosstunnel.h"
#include "credits.h"
#include "teapot.h"
#include "planes.h"
#include "hellogl.h"

void demo_init(void) {
    crosstunnel_init();
    plasma_init();
    rotozoom_init();
    boxes_init();
    credits_init();
    teapot_init();
    planes_init();
    hellogl_init();
}

void demo_frame(uint32_t *pixels, uint32_t time) {
    hellogl_frame(pixels, time);
    /*
    uint32_t scene = (time / 5486) % 7;
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
        case 3:
            credits_frame(pixels, time);
            break;
        case 4:
            teapot_frame(pixels, time);
            break;
        case 5:
            planes_frame(pixels, time);
            break;
        default:
            plasma_frame(pixels, time);
    }
    */
}
