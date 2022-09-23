#include <stdio.h>

#define PL_MPEG_IMPLEMENTATION
#include "pl_mpeg.h"
#include "video.h"

plm_t *plm;
plm_frame_t *plm_frame;
double next_frame_time = 0;
double ms_per_frame;

void video_init(void) {
    plm = plm_create_with_filename("../assets/pharmasign1.mpg");
    if (plm == NULL) {
        printf("Could not load video\n");
        return;
    }
    ms_per_frame = 1000.0 / plm_get_framerate(plm);
    plm_set_audio_enabled(plm, 0);
}

void video_get_frame() {
    plm_frame = plm_decode_video(plm);
}

void video_show_frame(uint32_t *pixels) {
    /* use abgr on the assumption that we're on a little-endian architecture */
    if (plm_frame != NULL) plm_frame_to_abgr(plm_frame, (uint8_t *)pixels, 192*4);
}

void video_frame(uint32_t *pixels, uint32_t time) {
    double td = (double)time;
    if (next_frame_time == 0 || (td - next_frame_time > 1000)) {
        /* first frame (or more than one second since the last call) */
        video_get_frame();
        video_show_frame(pixels);
        next_frame_time = td + ms_per_frame;
    } else {
        int got_frame = 0;
        while (next_frame_time <= td) {
            video_get_frame();
            got_frame = 1;
            next_frame_time += ms_per_frame;
        }
        if (got_frame) video_show_frame(pixels);
    }
}
