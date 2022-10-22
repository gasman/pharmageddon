#include <stdio.h>

#include "video.h"
#include "video1.h"

video vid1;

void video1_init(void) {
    video_open("../assets/ambulancefx.mpg", &vid1);
}

void video1_frame(uint32_t *pixels, uint32_t time) {
    video_frame(&vid1, pixels, time);
}
