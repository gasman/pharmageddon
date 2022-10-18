#include "credits.h"
#include "ambulance.h"
#include "planes.h"
#include "boards.h"
#include "video1.h"
#include "scroller.h"
#include "title.h"
#include "patarty.h"
#include "jarig.h"
#include "physics.h"
#include "badapple.h"

void demo_init(void) {
    credits_init();
    ambulance_init();
    planes_init();
    boards_init();
    video1_init();
    scroller_init();
    title_init();
    patarty_init();
    jarig_init();
    physics_init();
    badapple_init();
}

void demo_frame(uint32_t *pixels, uint32_t time) {
    title_frame(pixels, time);
    /*
    uint32_t scene = (time / 5486) % 7;
    switch (scene) {
        case 0:
            scroller_frame(pixels, time);
            break;
        case 1:
            title_frame(pixels, time);
            break;
        case 2:
            video1_frame(pixels, time);
            break;
        case 3:
            credits_frame(pixels, time);
            break;
        case 4:
            ambulance_frame(pixels, time);
            break;
        case 5:
            planes_frame(pixels, time);
            break;
        default:
            boards_frame(pixels, time);
    }
    */
}
