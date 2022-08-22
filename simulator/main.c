#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "demo.h"

#define SCALE 3
#define WIDTH 192
#define HEIGHT 192

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        printf("Error initializing SDL: %s\n", SDL_GetError());
        return -1;
    }

    int mixer_flags = MIX_INIT_MP3;
    int mixer_result = Mix_Init(mixer_flags);
    if (mixer_flags != mixer_result) {
        printf("Error initialising SDL mixer (result: %d).\n", mixer_result);
        printf("Mix_Init: %s\n", Mix_GetError());
        return -1;
    }

    Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640);
    Mix_Music *music = Mix_LoadMUS("../assets/midi-massacre.mp3");

    SDL_Window *window = SDL_CreateWindow(
        "Pharmageddon", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIDTH * SCALE, HEIGHT * SCALE,
        0
    );
    if (!window) {
        printf("Failed to create window: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window,
        -1, SDL_RENDERER_SOFTWARE);
    SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);
    SDL_RenderSetIntegerScale(renderer, 1);

    SDL_Texture *screen_texture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING,
        WIDTH, HEIGHT);

    uint32_t *pixels = malloc(WIDTH * HEIGHT * 4);

    SDL_Surface *window_surface = SDL_GetWindowSurface(window);
    if (!window_surface) {
        printf("Failed to get window surface: %s\n", SDL_GetError());
        return -1;
    }

    demo_init();

    int running = 1;
    Mix_PlayMusic(music, 1);

    while (running) {
        SDL_Event e;
        while(SDL_PollEvent(&e) > 0) {
            if (e.type == SDL_QUIT) {
                running = 0;
                break;
            }
        }

        uint32_t time = SDL_GetTicks();
        uint32_t crap_time = (uint32_t)(Mix_GetMusicPosition(music) * 1000);
        printf("time: %d vs %d\n", time, crap_time);

        demo_frame(pixels, time);

        /* blank out pixels outside the cross */
        for (int y = 0; y < 64; y++) {
            for (int x = 0; x < 64; x++) {
                pixels[x + y * WIDTH] = 0;
                pixels[x + 128 + y * WIDTH] = 0;
                pixels[x + (y + 128) * WIDTH] = 0;
                pixels[x + 128 + (y + 128) * WIDTH] = 0;
            }
        }

        SDL_RenderClear(renderer);
        SDL_UpdateTexture(screen_texture, NULL, pixels, WIDTH * 4);
        SDL_RenderCopy(renderer, screen_texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    Mix_FreeMusic(music);
    SDL_Quit(); 
    return 0;
}
