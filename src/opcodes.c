#include "chip8.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void chip8_execute_opcode(Chip8* chip) {
    uint16_t opcode = chip->opcode;
    uint16_t nnn = opcode & 0x0FFF;
    uint8_t  x   = (opcode >> 8) & 0x0F;
    uint8_t  y   = (opcode >> 4) & 0x0F;
    uint8_t  n   = opcode & 0x000F;
    uint8_t  kk  = opcode & 0x00FF;

    switch (opcode & 0xF000) {
        case 0x0000:
            if (opcode == 0x00E0) {
                memset(chip->display, 0, sizeof(chip->display));
                chip->draw_flag = true;
            } else if (opcode == 0x00EE) {
                chip->SP--;
                chip->PC = chip->stack[chip->SP];
            }
            break;

        case 0x1000: chip->PC = nnn; break;
        case 0x2000:
            chip->stack[chip->SP] = chip->PC;
            chip->SP++;
            chip->PC = nnn;
            break;
        case 0x3000:
            if (chip->V[x] == kk) chip->PC += 2;
            break;
        case 0x4000:
            if (chip->V[x] != kk) chip->PC += 2;
            break;
        case 0x5000:
            if (chip->V[x] == chip->V[y]) chip->PC += 2;
            break;
        case 0x6000: chip->V[x] = kk; break;
        case 0x7000: chip->V[x] += kk; break;

        case 0x8000:
            switch (n) {
                case 0x0: chip->V[x] = chip->V[y]; break;
                case 0x1: chip->V[x] |= chip->V[y]; break;
                case 0x2: chip->V[x] &= chip->V[y]; break;
                case 0x3: chip->V[x] ^= chip->V[y]; break;
                case 0x4: {
                    uint16_t sum = chip->V[x] + chip->V[y];
                    chip->V[0xF] = (sum > 0xFF) ? 1 : 0;
                    chip->V[x] = sum & 0xFF;
                    break;
                }
                case 0x5:
                    chip->V[0xF] = (chip->V[x] >= chip->V[y]) ? 1 : 0;
                    chip->V[x] -= chip->V[y];
                    break;
                case 0x6:
                    chip->V[0xF] = chip->V[y] & 0x01;
                    chip->V[x] = chip->V[y] >> 1;
                    break;
                case 0x7:
                    chip->V[0xF] = (chip->V[y] >= chip->V[x]) ? 1 : 0;
                    chip->V[x] = chip->V[y] - chip->V[x];
                    break;
                case 0xE:
                    chip->V[0xF] = (chip->V[y] >> 7) & 0x01;
                    chip->V[x] = chip->V[y] << 1;
                    break;
                default:
                    fprintf(stderr, "Unknown opcode: 0x%04X\n", opcode);
                    break;
            }
            break;

        case 0x9000:
            if (chip->V[x] != chip->V[y]) chip->PC += 2;
            break;
        case 0xA000: chip->I = nnn; break;
        case 0xB000: chip->PC = nnn + chip->V[0]; break;
        case 0xC000: chip->V[x] = (rand() % 256) & kk; break;

        case 0xD000: {
            uint8_t vx = chip->V[x] % CHIP8_DISPLAY_WIDTH;
            uint8_t vy = chip->V[y] % CHIP8_DISPLAY_HEIGHT;
            chip->V[0xF] = 0;

            for (int row = 0; row < n; row++) {
                uint8_t sprite_byte = chip->memory[chip->I + row];
                for (int col = 0; col < 8; col++) {
                    uint8_t pixel = (sprite_byte >> (7 - col)) & 1;
                    if (pixel == 0) continue;

                    int dx = (vx + col) % CHIP8_DISPLAY_WIDTH;
                    int dy = (vy + row) % CHIP8_DISPLAY_HEIGHT;

                    if (chip->display[dx][dy] == 1) {
                        chip->V[0xF] = 1;
                    }
                    chip->display[dx][dy] ^= 1;
                }
            }
            chip->draw_flag = true;
            break;
        }

        case 0xE000:
            if (kk == 0x9E) {
                if (chip->keypad[chip->V[x] & 0x0F]) chip->PC += 2;
            } else if (kk == 0xA1) {
                if (!chip->keypad[chip->V[x] & 0x0F]) chip->PC += 2;
            }
            break;

        case 0xF000:
            switch (kk) {
                case 0x07: chip->V[x] = chip->delay_timer; break;
                case 0x0A: {
                    bool key_pressed = false;
                    for (int i = 0; i < CHIP8_KEYPAD_SIZE; i++) {
                        if (chip->keypad[i]) {
                            chip->V[x] = i;
                            key_pressed = true;
                            break;
                        }
                    }
                    if (!key_pressed) {
                        chip->PC -= 2;
                    }
                    break;
                }
                case 0x15: chip->delay_timer = chip->V[x]; break;
                case 0x18: chip->sound_timer = chip->V[x]; break;
                case 0x1E: chip->I += chip->V[x]; break;
                case 0x29: chip->I = 0x050 + (chip->V[x] & 0x0F) * 5; break;
                case 0x33:
                    chip->memory[chip->I]     = chip->V[x] / 100;
                    chip->memory[chip->I + 1] = (chip->V[x] / 10) % 10;
                    chip->memory[chip->I + 2] = chip->V[x] % 10;
                    break;
                case 0x55:
                    for (int i = 0; i <= x; i++) {
                        chip->memory[chip->I + i] = chip->V[i];
                    }
                    break;
                case 0x65:
                    for (int i = 0; i <= x; i++) {
                        chip->V[i] = chip->memory[chip->I + i];
                    }
                    break;
                default:
                    fprintf(stderr, "Unknown opcode: 0x%04X\n", opcode);
                    break;
            }
            break;

        default:
            fprintf(stderr, "Unknown opcode: 0x%04X\n", opcode);
            break;
    }
}
