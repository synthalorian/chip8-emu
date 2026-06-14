#include "chip8.h"
#include <stdio.h>
#include <string.h>

extern void chip8_execute_opcode(Chip8* chip);

void chip8_init(Chip8* chip) {
    memset(chip, 0, sizeof(Chip8));
    chip->PC = CHIP8_PROGRAM_START;
    chip->running = true;

    // Load fontset into memory at 0x050
    memcpy(chip->memory + 0x050, CHIP8_FONTSET, CHIP8_FONTSET_SIZE);
}

bool chip8_load_rom(Chip8* chip, const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        perror("Failed to open ROM");
        return false;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (size > (CHIP8_MEMORY_SIZE - CHIP8_PROGRAM_START)) {
        fprintf(stderr, "ROM too large: %ld bytes\n", size);
        fclose(file);
        return false;
    }

    size_t read = fread(chip->memory + CHIP8_PROGRAM_START, 1, size, file);
    fclose(file);

    if ((long)read != size) {
        fprintf(stderr, "Failed to read ROM fully\n");
        return false;
    }

    printf("Loaded ROM: %s (%ld bytes)\n", filename, size);
    return true;
}

void chip8_set_key(Chip8* chip, uint8_t key, bool pressed) {
    if (key < CHIP8_KEYPAD_SIZE) {
        chip->keypad[key] = pressed ? 1 : 0;
    }
}

void chip8_update_timers(Chip8* chip) {
    if (chip->delay_timer > 0) {
        chip->delay_timer--;
    }
    if (chip->sound_timer > 0) {
        chip->sound_timer--;
    }
}

void chip8_emulate_cycle(Chip8* chip) {
    chip->opcode = (chip->memory[chip->PC] << 8) | chip->memory[chip->PC + 1];
    chip->PC += 2;
    chip8_execute_opcode(chip);
}
