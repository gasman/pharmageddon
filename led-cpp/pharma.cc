// -*- mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; -*-
// Small example how to use the library.
// For more examples, look at demo-main.cc
//
// This code is public domain
// (but note, that the led-matrix library this depends on is GPL v2)

#include "led-matrix.h"

#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

#include <math.h>

void effect(uint32_t *pixels, uint32_t time) {
    double dt = ((double)time) / 499;

    for (int y = 0; y < 192; y++) {
        for (int x = 0; x < 192; x++) {
            double dx = (double)x;
            double dy = (double)y;
            double v = sin(dx/50+dt)+sin(dy/22+dt)+sin(dx/16);

            pixels[x + y * 192] = (((int)(128 + v * 64)) & 255) << 24;
        }
    }
}

using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;

volatile bool interrupt_received = false;
static void InterruptHandler(int signo) {
  interrupt_received = true;
}

static void DrawOnCanvas(Canvas *canvas, uint32_t *pixels) {
  /*
   * Let's create a simple animation. We use the canvas to draw
   * pixels. We wait between each step to have a slower animation.
   */
  canvas->Fill(0, 0, 255);

  while (!interrupt_received) {
    uint32_t time = (uint32_t)(clock() * 1000 / CLOCKS_PER_SEC);
    effect(pixels, time);

    /* Top panel */
    for (int y = 0; y < 64; y++) {
      for (int x = 0; x < 64; x++) {
        uint32_t pix = pixels[x + 64 + y * 192];
        canvas->SetPixel(x + 64, y, pix >> 24, (pix >> 16) & 0xff, (pix >> 8) & 0xff);
      }
    }

    /* Left panel */
    for (int y = 0; y < 64; y++) {
      for (int x = 0; x < 64; x++) {
        uint32_t pix = pixels[x + (y + 64) * 192];
        canvas->SetPixel(x, y, pix >> 24, (pix >> 16) & 0xff, (pix >> 8) & 0xff);
      }
    }

    /* Middle + right panels */
    for (int y = 0; y < 64; y++) {
      for (int x = 0; x < 128; x++) {
        uint32_t pix = pixels[x + 64 + (y + 64) * 192];
        canvas->SetPixel(x, y + 64, pix >> 24, (pix >> 16) & 0xff, (pix >> 8) & 0xff);
      }
    }

    /* Bottom panel */
    for (int y = 0; y < 64; y++) {
      for (int x = 0; x < 64; x++) {
        uint32_t pix = pixels[x + 64 + (y + 128) * 192];
        canvas->SetPixel(x + 64, y + 128, pix >> 24, (pix >> 16) & 0xff, (pix >> 8) & 0xff);
      }
    } 
  }
}

int main(int argc, char *argv[]) {
  RGBMatrix::Options defaults;
  defaults.hardware_mapping = "regular";  // or e.g. "adafruit-hat"
  defaults.rows = 32;
  defaults.chain_length = 1;
  defaults.parallel = 1;
  defaults.show_refresh_rate = true;
  Canvas *canvas = RGBMatrix::CreateFromFlags(&argc, &argv, &defaults);
  if (canvas == NULL)
    return 1;

  // It is always good to set up a signal handler to cleanly exit when we
  // receive a CTRL-C for instance. The DrawOnCanvas() routine is looking
  // for that.
  signal(SIGTERM, InterruptHandler);
  signal(SIGINT, InterruptHandler);

  uint32_t *pixels = (uint32_t *)malloc(192 * 192 * 4);

  DrawOnCanvas(canvas, pixels);    // Using the canvas.

  // Animation finished. Shut down the RGB matrix.
  canvas->Clear();
  delete canvas;

  return 0;
}
