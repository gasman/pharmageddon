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

#define BAR 3840

#define SCROLLER_END BAR * 7
#define BOARDS_END BAR * 9
#define CREDITS_END BAR * 13
#define TITLE_END BAR * 17
#define AMBULANCE_END BAR * 22
#define EFFECT1_END BAR * 26
#define MEMES 118000
#define MEMES_BAR 3200
#define MEME1_END MEMES + MEMES_BAR * 1
#define MEME2_END MEMES + MEMES_BAR * 2
#define MEME3_END MEMES + MEMES_BAR * 3
#define MEME4_END MEMES + MEMES_BAR * 4
#define MEME5_END MEMES + MEMES_BAR * 5
#define MEME6_END MEMES + MEMES_BAR * 6
#define MEME7_END MEMES + MEMES_BAR * 7
#define MEME8_END MEMES + MEMES_BAR * 8

void demo_frame(uint32_t *pixels, uint32_t time) {
    if (time < SCROLLER_END) {
        scroller_frame(pixels, time);
    } else if (time < BOARDS_END) {
        boards_frame(pixels, time);
    } else if (time < CREDITS_END) {
        credits_frame(pixels, time - BOARDS_END);
    } else if (time < TITLE_END) {
        title_frame(pixels, time - CREDITS_END);
    } else if (time < AMBULANCE_END) {
        ambulance_frame(pixels, time - TITLE_END);
    } else if (time < EFFECT1_END) {
        twister_frame(pixels, time - AMBULANCE_END);
    } else if (time < MEMES) {
        prescription_frame(pixels, time - EFFECT1_END);
    } else if (time < MEME1_END) {
        amigaball_frame(pixels, time - MEMES);
    } else if (time < MEME2_END) {
        jarig_frame(pixels, time);
    } else if (time < MEME3_END) {
        patarty_frame(pixels, time);
    } else if (time < MEME4_END) {
        badapple_frame(pixels, time);
    } else if (time < MEME5_END) {
        nyancat_frame(pixels, time);
    } else if (time < MEME6_END) {
        jarig_frame(pixels, time);  // REPLACE
    } else if (time < MEME7_END) {
        patarty_frame(pixels, time);  // REPLACE
    } else if (time < MEME8_END) {
        stniccc_frame(pixels, time - MEME7_END);
    } else {
        video1_frame(pixels, time - MEME8_END);
    }
}
