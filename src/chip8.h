#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>
#include <stdbool.h>

#define CHIP8_MEMORY_SIZE 4096
#define CHIP8_DISPLAY_WIDTH 64
#define CHIP8_DISPLAY_HEIGHT 32
#define CHIP8_STACK_SIZE 16
#define CHIP8_KEYPAD_SIZE 16
#define CHIP8_FONTSET_SIZE 80
#define CHIP8_PROGRAM_START 0x200

// Font sprites loaded at 0x050
static const uint8_t CHIP8_FONTSET[CHIP8_FONTSET_SIZE] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

typedef struct {
    uint8_t  memory[CHIP8_MEMORY_SIZE];
    uint8_t  V[16];               // registers V0-VF
    uint16_t I;                   // index register
    uint16_t PC;                  // program counter
    uint8_t  SP;                  // stack pointer
    uint16_t stack[CHIP8_STACK_SIZE];
    uint8_t  delay_timer;
    uint8_t  sound_timer;
    uint8_t  display[CHIP8_DISPLAY_WIDTH][CHIP8_DISPLAY_HEIGHT]; // [x][y]
    uint8_t  keypad[CHIP8_KEYPAD_SIZE];
    uint16_t opcode;
    bool     draw_flag;           // true when display needs redraw
    bool     running;
} Chip8;

void chip8_init(Chip8* chip);
bool chip8_load_rom(Chip8* chip, const char* filename);
void chip8_emulate_cycle(Chip8* chip);
void chip8_update_timers(Chip8* chip);
void chip8_set_key(Chip8* chip, uint8_t key, bool pressed);

#endif
