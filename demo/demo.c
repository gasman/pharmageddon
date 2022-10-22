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
#include "twister.h"
#include "stniccc.h"
#include "prescription.h"
#include "nyancat.h"
#include "amigaball.h"

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
    twister_init();
    stniccc_init();
    prescription_init();
    nyancat_init();
    amigaball_init();
}

#define BAR 5486
#define MEMES BAR * 21
#define MEME_BAR 4543

void demo_frame(uint32_t *pixels, uint32_t time) {
    ambulance_frame(pixels, time);
    /*
    if (time < BAR * 3) {
        scroller_frame(pixels, time);
    } else if (time < BAR * 7) {
        credits_frame(pixels, time - BAR * 3);
    } else if (time < BAR * 9) {
        title_frame(pixels, time - BAR * 7);
    } else if (time < BAR * 11) {
        ambulance_frame(pixels, time);
    } else if (time < BAR * 13) {
        planes_frame(pixels, time);
    } else if (time < BAR * 15) {
        video1_frame(pixels, time);
    } else if (time < BAR * 18) {
        twister_frame(pixels, time - BAR * 15);
    } else if (time < MEMES) {
        prescription_frame(pixels, time - BAR * 18);
    } else if (time < MEMES + MEME_BAR) {
        jarig_frame(pixels, time);
    } else if (time < MEMES + MEME_BAR * 2) {
        patarty_frame(pixels, time);
    } else if (time < MEMES + MEME_BAR * 3) {
        badapple_frame(pixels, time);
    } else if (time < MEMES + MEME_BAR * 4) {
        stniccc_frame(pixels, time - (MEMES + MEME_BAR * 3));
    } else {
        boards_frame(pixels, time);
    }
    */
    /*
    uint32_t scene = (time / 5486) % 7;
    switch (scene) {
        case 0:
            scroller_frame(pixels, time);
            break;
        default:
            boards_frame(pixels, time);
    }
    */
}
