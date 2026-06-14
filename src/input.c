#include "chip8.h"
#include <SDL2/SDL.h>

// CHIP-8 keypad layout:
// 1 2 3 C
// 4 5 6 D
// 7 8 9 E
// A 0 B F
//
// Mapped to keyboard:
// 1 2 3 4
// Q W E R
// A S D F
// Z X C V

static uint8_t map_key(SDL_Keycode key) {
    switch (key) {
        case SDLK_1: return 0x1;
        case SDLK_2: return 0x2;
        case SDLK_3: return 0x3;
        case SDLK_4: return 0xC;
        case SDLK_q: return 0x4;
        case SDLK_w: return 0x5;
        case SDLK_e: return 0x6;
        case SDLK_r: return 0xD;
        case SDLK_a: return 0x7;
        case SDLK_s: return 0x8;
        case SDLK_d: return 0x9;
        case SDLK_f: return 0xE;
        case SDLK_z: return 0xA;
        case SDLK_x: return 0x0;
        case SDLK_c: return 0xB;
        case SDLK_v: return 0xF;
        default:     return 0xFF;
    }
}

void input_handle(Chip8* chip) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                chip->running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    chip->running = false;
                } else {
                    uint8_t key = map_key(event.key.keysym.sym);
                    if (key != 0xFF) {
                        chip8_set_key(chip, key, true);
                    }
                }
                break;
            case SDL_KEYUP: {
                uint8_t key = map_key(event.key.keysym.sym);
                if (key != 0xFF) {
                    chip8_set_key(chip, key, false);
                }
                break;
            }
        }
    }
}
