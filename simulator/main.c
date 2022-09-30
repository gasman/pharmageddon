#define GL_GLEXT_PROTOTYPES

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_opengl.h>

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

    Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 640);
    Mix_Music *music = Mix_LoadMUS("../assets/midi-massacre.mp3");

    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 0 );
    SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, 1 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

    SDL_Window *window = SDL_CreateWindow(
        "Pharmageddon", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIDTH * SCALE, HEIGHT * SCALE,
        0
    );
    if (!window) {
        printf("Failed to create window: %s\n", SDL_GetError());
        return -1;
    }
    SDL_GLContext context = SDL_GL_CreateContext( window );

    GLuint renderbuffer, framebuffer, depthbuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glGenRenderbuffers(1, &renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, 192, 192);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbuffer);

    glGenRenderbuffers(1, &depthbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, 192, 192);
    /* TODO: set up stencil buffer too? */
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbuffer);

    SDL_Renderer *renderer = SDL_CreateRenderer(window,
        -1, SDL_RENDERER_SOFTWARE);
    SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);
    SDL_RenderSetIntegerScale(renderer, 1);

    SDL_Texture *screen_texture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING,
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
    uint32_t audio_start_time = SDL_GetTicks();

    while (running) {
        SDL_Event e;
        while(SDL_PollEvent(&e) > 0) {
            if (e.type == SDL_QUIT) {
                running = 0;
                break;
            }
        }

        uint32_t time = SDL_GetTicks() - audio_start_time;

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
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
