#include "credits.h"
#include "ambulance.h"
#include "planes.h"
#include "boards.h"
#include "video.h"
#include "scroller.h"

void demo_init(void) {
    credits_init();
    ambulance_init();
    planes_init();
    boards_init();
    video_init();
    scroller_init();
}

void demo_frame(uint32_t *pixels, uint32_t time) {
    uint32_t scene = (time / 5486) % 6;
    switch (scene) {
        case 0:
            scroller_frame(pixels, time);
            break;
        case 1:
            video_frame(pixels, time);
            break;
        case 2:
            credits_frame(pixels, time);
            break;
        case 3:
            ambulance_frame(pixels, time);
            break;
        case 4:
            planes_frame(pixels, time);
            break;
        default:
            boards_frame(pixels, time);
    }
}
