#include "chip8.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

extern bool display_init(void);
extern void display_render(Chip8* chip);
extern void display_cleanup(void);

extern void input_handle(Chip8* chip);

extern bool audio_init(Chip8* chip);
extern void audio_cleanup(void);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <rom.ch8>\n", argv[0]);
        return 1;
    }

    srand((unsigned)time(NULL));

    Chip8 chip;
    chip8_init(&chip);

    if (!chip8_load_rom(&chip, argv[1])) {
        return 1;
    }

    if (!display_init()) {
        return 1;
    }

    if (!audio_init(&chip)) {
        display_cleanup();
        return 1;
    }

    uint32_t last_cpu_tick = SDL_GetTicks();
    uint32_t last_timer_tick = SDL_GetTicks();
    uint32_t last_render_tick = SDL_GetTicks();

    const uint32_t CPU_INTERVAL_MS = 2;       // ~500 Hz
    const uint32_t TIMER_INTERVAL_MS = 16;    // ~60 Hz
    const uint32_t RENDER_INTERVAL_MS = 16;   // ~60 FPS

    while (chip.running) {
        uint32_t now = SDL_GetTicks();

        // CPU cycles: ~500 Hz
        while (now - last_cpu_tick >= CPU_INTERVAL_MS) {
            chip8_emulate_cycle(&chip);
            last_cpu_tick += CPU_INTERVAL_MS;
        }

        // Timers: 60 Hz
        if (now - last_timer_tick >= TIMER_INTERVAL_MS) {
            chip8_update_timers(&chip);
            last_timer_tick += TIMER_INTERVAL_MS;
        }

        // Render display
        if (now - last_render_tick >= RENDER_INTERVAL_MS) {
            display_render(&chip);
            last_render_tick += RENDER_INTERVAL_MS;
        }

        // Input
        input_handle(&chip);

        SDL_Delay(1);
    }

    audio_cleanup();
    display_cleanup();
    return 0;
}
