# Pharmageddon

## by The Paramedics

a demo for a pharmacy LED sign
presented at TRSAC 2022


## The Paramedics are...

* Gasman - Code, hardware
* Nico - Code
* RaccoonViolet - Graphics
* Stormcaller - Visuals
* Superogue - Code
* TôBach - Music

## Active ingredients

* 1 x Raspberry Pi 3
* 5 x LED matrix panels, 64x64 pixels
* 1 x [Electrodragon panel drive board](https://www.electrodragon.com/product/rgb-matrix-panel-drive-board-raspberry-pi/)
* 1 x 300W power supply

To be consumed with [rpi-rgb-led-matrix](https://github.com/hzeller/rpi-rgb-led-matrix), [assimp](https://assimp-docs.readthedocs.io/en/v5.1.0/), and [SDL2](https://www.libsdl.org/).


## Building

To run on the real hardware, cd to the `led` directory, run `make`, then launch the `pharma` executable with:

    sudo ./pharma --led-rows=64 --led-cols=64 --led-parallel=3 --led-slowdown-gpio=2 --led-pwm-bits=8 --led-chain=2 --led-show-refresh

To run the SDL-based simulator, cd to the `simulator` directory, run `make`, then launch the `pharma` executable.
