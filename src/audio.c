#include "chip8.h"
#include <SDL2/SDL.h>
#include <stdint.h>
#include <string.h>

static SDL_AudioDeviceID audio_device = 0;

#define AUDIO_SAMPLE_RATE 44100
#define AUDIO_FREQUENCY   440

static void audio_callback(void* userdata, Uint8* stream, int len) {
    Chip8* chip = (Chip8*)userdata;
    int16_t* buffer = (int16_t*)stream;
    int samples = len / sizeof(int16_t);

    static uint32_t phase = 0;
    const uint32_t period = AUDIO_SAMPLE_RATE / AUDIO_FREQUENCY;

    bool beep = (chip->sound_timer > 0);

    for (int i = 0; i < samples; i++) {
        if (beep) {
            buffer[i] = (phase < (period / 2)) ? 3000 : -3000;
            phase = (phase + 1) % period;
        } else {
            buffer[i] = 0;
            phase = 0;
        }
    }
}

bool audio_init(Chip8* chip) {
    if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "SDL audio init failed: %s\n", SDL_GetError());
        return false;
    }

    SDL_AudioSpec desired, obtained;
    memset(&desired, 0, sizeof(desired));
    desired.freq = AUDIO_SAMPLE_RATE;
    desired.format = AUDIO_S16SYS;
    desired.channels = 1;
    desired.samples = 512;
    desired.callback = audio_callback;
    desired.userdata = chip;

    audio_device = SDL_OpenAudioDevice(NULL, 0, &desired, &obtained, 0);
    if (audio_device == 0) {
        fprintf(stderr, "SDL audio device open failed: %s\n", SDL_GetError());
        return false;
    }

    SDL_PauseAudioDevice(audio_device, 0);
    return true;
}

void audio_cleanup(void) {
    if (audio_device) {
        SDL_CloseAudioDevice(audio_device);
        audio_device = 0;
    }
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}
