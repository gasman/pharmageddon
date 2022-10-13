#include "plasma.h"
#include "boxes.h"
#include "rotozoom.h"
#include "crosstunnel.h"
#include "credits.h"
#include "teapot.h"
#include "planes.h"
#include "boards.h"
#include "video.h"

void demo_init(void) {
    crosstunnel_init();
    plasma_init();
    rotozoom_init();
    boxes_init();
    credits_init();
    teapot_init();
    planes_init();
    boards_init();
    video_init();
}

void demo_frame(uint32_t *pixels, uint32_t time) {
    uint32_t scene = (time / 5486) % 5;
    switch (scene) {
        case 0:
            crosstunnel_frame(pixels, time);
            break;
        case 1:
            video_frame(pixels, time);
            break;
        case 2:
            credits_frame(pixels, time);
            break;
        case 3:
            teapot_frame(pixels, time);
            break;
        case 4:
            planes_frame(pixels, time);
            break;
        default:
            boards_frame(pixels, time);
    }
}
