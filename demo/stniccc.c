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

    while (1) {
        printf("frame %d\n", frame_index);

        unsigned char flags = *ptr++;
        stniccc_video_frame frame = frames[frame_index];
        frame.clear_screen = flags & 0x01;
        unsigned char has_palette = flags & 0x02;
        frame.is_indexed = flags & 0x04;
        if (has_palette) {
            printf("reading palette\n");
            unsigned short palette_bits = ((unsigned short)(*ptr++)) << 8;
            palette_bits |= *ptr++;
            for (int palette_index = 0; palette_index < 16; palette_index++) {
                if (palette_bits & 0x8000) {
                    unsigned char r = *ptr++;
                    unsigned char gb = *ptr++;
                    palette[palette_index] = (
                        (((uint32_t)(r & 0x07)) << 24)
                        | (((uint32_t)(gb & 0x70)) << 12)
                        | (((uint32_t)(gb & 0x07)) << 8)
                    );
                }
                palette_bits <<= 1;
            }
        }
        memcpy(frame.palette, palette, 16);

        unsigned char descriptor;
        if (frame.is_indexed) {
            unsigned char vertex_count = *ptr++;
            printf("indexed - %d vertices\n", vertex_count);
            frame.vertex_data = ptr;
            ptr += vertex_count * 2;

            frame.poly_data = ptr;
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
            printf("non-indexed\n");
            frame.poly_data = ptr;
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
    }
}

void stniccc_frame(uint32_t *pixels, uint32_t time) {
}
