#include "chip8.h"
#include <SDL2/SDL.h>
#include <stdio.h>

#define DISPLAY_SCALE 12
#define WINDOW_WIDTH  (CHIP8_DISPLAY_WIDTH  * DISPLAY_SCALE)
#define WINDOW_HEIGHT (CHIP8_DISPLAY_HEIGHT * DISPLAY_SCALE)

static SDL_Window*   window = NULL;
static SDL_Renderer* renderer = NULL;

bool display_init(void) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL video init failed: %s\n", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow(
        "CHIP-8 Emulator",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        fprintf(stderr, "SDL window creation failed: %s\n", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "SDL renderer creation failed: %s\n", SDL_GetError());
        return false;
    }

    SDL_RenderSetLogicalSize(renderer, CHIP8_DISPLAY_WIDTH, CHIP8_DISPLAY_HEIGHT);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    return true;
}

void display_render(Chip8* chip) {
    if (!chip->draw_flag) return;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (int y = 0; y < CHIP8_DISPLAY_HEIGHT; y++) {
        for (int x = 0; x < CHIP8_DISPLAY_WIDTH; x++) {
            if (chip->display[x][y]) {
                SDL_Rect pixel = { x, y, 1, 1 };
                SDL_RenderFillRect(renderer, &pixel);
            }
        }
    }

    SDL_RenderPresent(renderer);
    chip->draw_flag = false;
}

void display_cleanup(void) {
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window)   SDL_DestroyWindow(window);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}
