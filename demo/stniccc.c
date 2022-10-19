#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gfx.h"
#include "stniccc.h"

#define STNICCC_DATA_LEN 639976
#define STNICCC_FRAME_COUNT 1800

typedef struct stniccc_video_frame {
    unsigned char clear_screen;
    unsigned char is_indexed;
    uint32_t palette[16];
    unsigned char *vertex_data;
    unsigned char *poly_data;
} stniccc_video_frame;

unsigned char stniccc_data[STNICCC_DATA_LEN];
stniccc_video_frame frames[STNICCC_FRAME_COUNT];

void stniccc_init(void) {
    FILE *f;
    f = fopen("../assets/stniccc.bin","rb");
    fread(stniccc_data, STNICCC_DATA_LEN, 1, f);
    fclose(f);

    uint32_t palette[16];
    for (int i = 0; i < 16; i++) {
        palette[i] = 0;
    }
    int frame_index = 0;
    unsigned char *ptr = stniccc_data;
    stniccc_video_frame *frame = frames;

    while (1) {
        unsigned char flags = *ptr++;
        frame->clear_screen = flags & 0x01;
        unsigned char has_palette = flags & 0x02;
        frame->is_indexed = flags & 0x04;
        if (has_palette) {
            unsigned short palette_bits = ((unsigned short)(*ptr++)) << 8;
            palette_bits |= *ptr++;
            for (int palette_index = 0; palette_index < 16; palette_index++) {
                if (palette_bits & 0x8000) {
                    unsigned char r = *ptr++;
                    unsigned char gb = *ptr++;
                    palette[palette_index] = (
                        (((uint32_t)(r & 0x07)) << 29)
                        | (((uint32_t)(gb & 0x70)) << 17)
                        | (((uint32_t)(gb & 0x07)) << 13)
                    );
                }
                palette_bits <<= 1;
            }
        }
        memcpy(frame->palette, palette, 16*4);

        unsigned char descriptor;
        if (frame->is_indexed) {
            unsigned char vertex_count = *ptr++;
            frame->vertex_data = ptr;
            ptr += vertex_count * 2;

            frame->poly_data = ptr;
            while (1) {
                descriptor = *ptr++;
                if (descriptor >= 0xfd) {
                    break;
                } else {
                    unsigned char poly_count = descriptor & 0x0f;
                    ptr += poly_count;
                }
            }
        } else {
            /* frame is non-indexed */
            frame->poly_data = ptr;
            while (1) {
                descriptor = *ptr++;
                if (descriptor >= 0xfd) {
                    break;
                } else {
                    unsigned char poly_count = descriptor & 0x0f;
                    ptr += poly_count * 2;
                }
            }
        }
        if (descriptor == 0xfd) {
            break;
        } else if (descriptor == 0xfe) {
            size_t offset = ptr - stniccc_data;
            offset = (offset & ~0xffff) + 0x10000;
            ptr = stniccc_data + offset;
        }
        frame_index++;
        frame++;
    }
}

void stniccc_frame(uint32_t *pixels, uint32_t time) {
    stniccc_video_frame frame = frames[time / 50];

    if (frame.clear_screen) {
        gfx_cls(pixels, 0x00000000);
    }
    if (frame.is_indexed) {
        unsigned char *ptr = frame.poly_data;
        while (1) {
            unsigned char descriptor = *ptr++;
            if (descriptor >= 0xfd) {
                break;
            }
            unsigned char colour_index = descriptor >> 4;
            uint32_t colour = frame.palette[colour_index];
            unsigned char v_count = descriptor & 0x0f;
            unsigned char v0_index = *ptr++;
            unsigned char *v0_data = frame.vertex_data + v0_index * 2;
            int v0x = v0_data[0];
            int v0y = v0_data[1];
            unsigned char v1_index = *ptr++;
            unsigned char *v1_data = frame.vertex_data + v1_index * 2;
            int v1x = v1_data[0];
            int v1y = v1_data[1];
            for (int v_index = 2; v_index < v_count; v_index++) {
                unsigned char v2_index = *ptr++;
                unsigned char *v2_data = frame.vertex_data + v2_index * 2;
                int v2x = v2_data[0];
                int v2y = v2_data[1];
                gfx_filltri(pixels, v0x, v0y, v1x, v1y, v2x, v2y, colour);
                v1x = v2x;
                v1y = v2y;
            }
        }
    } else {
        unsigned char *ptr = frame.poly_data;
        while (1) {
            unsigned char descriptor = *ptr++;
            if (descriptor >= 0xfd) {
                break;
            }
            uint32_t colour = frame.palette[descriptor >> 4];
            unsigned char v_count = descriptor & 0x0f;
            int v0x = *ptr++;
            int v0y = *ptr++;
            int v1x = *ptr++;
            int v1y = *ptr++;
            for (int v_index = 2; v_index < v_count; v_index++) {
                int v2x = *ptr++;
                int v2y = *ptr++;
                gfx_filltri(pixels, v0x, v0y, v1x, v1y, v2x, v2y, colour);
                v1x = v2x;
                v1y = v2y;
            }
        }
    }
}
