#include <stdio.h>

#include "video.h"
#include "video1.h"

video vid;

void video1_init(void) {
    video_open("../assets/pharmasign1.mpg", &vid);
}

void video1_frame(uint32_t *pixels, uint32_t time) {
    video_frame(&vid, pixels, time);
}
