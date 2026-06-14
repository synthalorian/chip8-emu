# SCOPE: CHIP-8 EMULATOR

## v1 — Core VM

- [ ] Implement all 35 CHIP-8 opcodes
- [ ] Memory: 4K byte array (0x000–0xFFF)
- [ ] Registers: `V[0..F]`, `I`, `PC`, `SP`, `stack[0..F]`
- [ ] Timers: delay timer (60Hz), sound timer (60Hz + beep)
- [ ] Display: 64×32 monochrome, XOR drawing, collision detection
- [ ] Keypad: 16 keys, blocking input for `FX0A`
- [ ] SDL2 window + audio beep
- [ ] Run test ROMs: `chip8-test-suite`, `ibm.ch8`, `pong.ch8`

## v2 — Debugger & UI

- [ ] Step mode: execute one opcode per keypress
- [ ] Disassembly: print current opcode + operands
- [ ] Register view: live dump of V0–VF, I, PC, SP, timers
- [ ] Memory dump: hex view around PC
- [ ] Breakpoints: set address, break on opcode, break on draw
- [ ] ROM loader UI: file picker or drag-and-drop
- [ ] Speed control: 1x, 2x, 4x, 8x, pause

## v3 — Super-CHIP8 & Polish

- [ ] Super-CHIP8 support: 128×64 display, extended opcodes, scroll
- [ ] Save states: save + load full VM state to disk
- [ ] Config file: key remapping, color schemes, audio on/off
- [ ] Quirks mode: toggle legacy behavior per-platform (SCHIP, CHIP-48, etc.)
- [ ] CRT shader overlay for retro feel (optional)

## Architecture

```
chip8-emu/
├── src/
│   ├── main.c             # entry, CLI, SDL2 init
│   ├── chip8.c/h          # core VM: memory, registers, opcodes
│   ├── opcodes.c/h        # opcode decode + execute table
│   ├── display.c/h        # 64×32 framebuffer, SDL2 render
│   ├── input.c/h          # keypad mapping, SDL2 events
│   ├── audio.c/h          # SDL2 beep on sound_timer > 0
│   ├── debugger.c/h       # v2: step, disasm, registers, breakpoints
│   └── loader.c/h         # ROM file read into memory
├── roms/                  # .ch8 files (not in repo)
├── tests/                 # test runner for opcode suite
├── Makefile
└── (Cargo.toml for Rust port)
```

### VM State

```c
typedef struct {
    uint8_t  memory[4096];     // 4K RAM
    uint8_t  V[16];            // general-purpose registers V0-VF
    uint16_t I;                // address register
    uint16_t PC;               // program counter
    uint8_t  SP;               // stack pointer
    uint16_t stack[16];        // subroutine stack
    uint8_t  delay_timer;      // decrements at 60Hz
    uint8_t  sound_timer;      // decrements at 60Hz, beeps while > 0
    uint8_t  display[64][32];  // monochrome framebuffer
    uint8_t  keypad[16];       // hex keypad state
    uint16_t opcode;           // current opcode
} Chip8;
```

### Opcode Fetch-Decode-Execute

```
1. opcode = memory[PC] << 8 | memory[PC + 1]
2. PC += 2
3. switch (opcode & 0xF000) → dispatch to handler
4. handler reads nibbles for X, Y, N, NN, NNN
```

## Milestones

| Day | Target |
|-----|--------|
| 1 | Decode all 35 opcodes, run `ibm.ch8` |
| 2 | Display: 64×32 render, XOR draw, collision |
| 3 | Input + sound: keypad, delay/sound timers, beep |
| 4 | Run games: Pong, Tetris, Space Invaders |
| 7 | Debugger: step, disasm, register view |

## Deferred / Future

- CHIP-8 assembler/disassembler standalone tool
- CHIP-8 compiler from a tiny C subset
- WebAssembly port (SDL2 + Emscripten)
- Networked multiplayer (ambitious, but fun)
