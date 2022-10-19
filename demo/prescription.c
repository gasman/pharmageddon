#include <stdlib.h>
#include "gfx.h"
#include "prescription.h"

gfx_image raccoondoctor;
gfx_image confusedperson;
gfx_image text1;
gfx_image text2;

void prescription_init(void) {
    gfx_loadimage("../assets/raccoon_doc.png", &raccoondoctor);
    gfx_loadimage("../assets/confused_person.png", &confusedperson);
    gfx_loadimage("../assets/prescriptiontext1.png", &text1);
    gfx_loadimage("../assets/prescriptiontext2.png", &text2);
}

#define SCREEN_1_TIME 6000

void prescription_frame(uint32_t *pixels, uint32_t time) {
    gfx_image pic, text;
    int scroller_x, scroller_screen_x, scroller_screen_y;
    if (time < SCREEN_1_TIME) {
        pic = raccoondoctor;
        text = text1;
        scroller_x = time / 18;
        scroller_screen_x = 74;
        scroller_screen_y = 21;
    } else {
        pic = confusedperson;
        text = text2;
        scroller_x = (time - SCREEN_1_TIME) / 18;
        scroller_screen_x = 74;
        scroller_screen_y = 29;
    }

    gfx_drawimage(pixels, &pic, 0, 0);

    uint32_t *screen_row = pixels + scroller_screen_y * 192 + scroller_screen_x;
    unsigned char *img_row = text.data + scroller_x * 4;
    for (int y = 0; y < 16; y++) {
        uint32_t *screen = screen_row;
        unsigned char *img = img_row;
        for (int x = 0; x < 42; x++) {
            *screen = (*img ? 0xf4f4f400 : 0x00000000);
            screen++;
            img += 4;
        }
        screen_row += 192;
        img_row += text.width * 4;
    }
}
