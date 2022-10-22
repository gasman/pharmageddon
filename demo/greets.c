#include <stdio.h>

#include "video.h"
#include "greets.h"

video greetsvid;

void greets_init(void) {
    video_open("../assets/pharma5.mpg", &greetsvid);
}

void greets_frame(uint32_t *pixels, uint32_t time) {
    video_frame(&greetsvid, pixels, time);
}
