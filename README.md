# Pharmageddon

## Project structure

* `demo` - actual demo code (shared by both the simulator and LED builds)
  * `demo.c`, `demo.h` - top-level initialisation and main loop. Call your effect code inside `demo_frame` if you want to see it...
  * `gfx.c`, `gfx.h` - graphics library (functions for line drawing and stuff)
  * `plasma.c`, `plasma.h`, `boxes.c`, `boxes.h` - test effects
* `led` - code for launching on the real LED display
* `simulator` - code for launching on the SDL-based simulator


## Building (SDL simulator)

You'll need libraries / development headers for SDL2 and Assimp.

From the `simulator` directory, run `make`. Then launch the `pharma` executable.
