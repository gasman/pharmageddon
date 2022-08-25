/* -*- mode: c; c-basic-offset: 2; indent-tabs-mode: nil; -*-
 *
 * Using the C-API of this library.
 *
 */

#include "led-matrix-c.h"

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "demo.h"

uint32_t pixels[192 * 192];

int main(int argc, char **argv) {
  struct RGBLedMatrixOptions options;
  struct RGBLedMatrix *matrix;
  struct LedCanvas *offscreen_canvas;
  int width, height;

  memset(&options, 0, sizeof(options));
  options.rows = 32;
  options.chain_length = 1;

  /* This supports all the led commandline options. Try --led-help */
  matrix = led_matrix_create_from_options(&options, &argc, &argv);
  if (matrix == NULL)
    return 1;

  /* Let's do an example with double-buffering. We create one extra
   * buffer onto which we draw, which is then swapped on each refresh.
   * This is typically a good aproach for animations and such.
   */
  offscreen_canvas = led_matrix_create_offscreen_canvas(matrix);

  led_canvas_get_size(offscreen_canvas, &width, &height);

  fprintf(stderr, "Size: %dx%d. Hardware gpio mapping: %s\n",
          width, height, options.hardware_mapping);

  if (SDL_Init(SDL_INIT_AUDIO) != 0) {
    printf("Error initializing SDL: %s\n", SDL_GetError());
    return -1;
  }

int i;

for (i = 0; i < SDL_GetNumAudioDrivers(); ++i) {
    printf("Audio driver %d: %s\n", i, SDL_GetAudioDriver(i));
}

int device_count = SDL_GetNumAudioDevices(0);
printf("Found %d audio devices\n", device_count);
printf("Audio device 0: %s\n", SDL_GetAudioDeviceName(0, 0));
for (i = 0; i < device_count; ++i) {
    printf("Audio device %d: %s\n", i, SDL_GetAudioDeviceName(i, 0));
}

/*
  int mixer_flags = MIX_INIT_MP3;
  int mixer_result = Mix_Init(mixer_flags);
  if (mixer_flags != mixer_result) {
    printf("Error initialising SDL mixer (result: %d).\n", mixer_result);
    printf("Mix_Init: %s\n", Mix_GetError());
    return -1;
  }

  Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 640);
  Mix_Music *music = Mix_LoadMUS("../assets/midi-massacre.mp3");
*/
  demo_init();

  // Mix_PlayMusic(music, 1);
  uint32_t audio_start_time = SDL_GetTicks();

  while (1) {
    uint32_t time = (uint32_t)(clock() * 1000 / CLOCKS_PER_SEC);
    demo_frame(pixels, time);

    /* Top panel */
    for (int y = 0; y < 64; y++) {
      for (int x = 0; x < 64; x++) {
        uint32_t pix = pixels[x + 64 + y * 192];
        led_canvas_set_pixel(offscreen_canvas, x + 64, y, pix >> 24, (pix >> 16) & 0xff, (pix >> 8) & 0xff);
      }
    }

    /* Left panel */
    for (int y = 0; y < 64; y++) {
      for (int x = 0; x < 64; x++) {
        uint32_t pix = pixels[x + (y + 64) * 192];
        led_canvas_set_pixel(offscreen_canvas, x, y, pix >> 24, (pix >> 16) & 0xff, (pix >> 8) & 0xff);
      }
    }

    /* Middle + right panels */
    for (int y = 0; y < 64; y++) {
      for (int x = 0; x < 128; x++) {
        uint32_t pix = pixels[x + 64 + (y + 64) * 192];
        led_canvas_set_pixel(offscreen_canvas, x, y + 64, pix >> 24, (pix >> 16) & 0xff, (pix >> 8) & 0xff);
      }
    }

    /* Bottom panel */
    for (int y = 0; y < 64; y++) {
      for (int x = 0; x < 64; x++) {
        uint32_t pix = pixels[x + 64 + (y + 128) * 192];
        led_canvas_set_pixel(offscreen_canvas, x + 64, y + 128, pix >> 24, (pix >> 16) & 0xff, (pix >> 8) & 0xff);
      }
    } 

    /* Now, we swap the canvas. We give swap_on_vsync the buffer we
     * just have drawn into, and wait until the next vsync happens.
     * we get back the unused buffer to which we'll draw in the next
     * iteration.
     */
    offscreen_canvas = led_matrix_swap_on_vsync(matrix, offscreen_canvas);
  }

  /*
   * Make sure to always call led_matrix_delete() in the end to reset the
   * display. Installing signal handlers for defined exit is a good idea.
   */
  led_matrix_delete(matrix);

  return 0;
}
