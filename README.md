# CHIP-8 EMULATOR

> A classic virtual machine. 35 opcodes. 4K RAM. 64×32 pixels. One beep.

## What Is CHIP-8?

CHIP-8 is an interpreted programming language from the mid-1970s, designed to run simple games on 8-bit microcomputers. It has a tiny instruction set, a monochrome display, and a hex keypad. Despite its simplicity, it runs Pong, Tetris, Space Invaders, and dozens of other classic games.

## Project Goals

- Build a complete, accurate CHIP-8 emulator from scratch.
- Run the standard test ROMs and classic games.
- Add a built-in debugger for stepping, disassembly, and register inspection.
- Extend to Super-CHIP8 for higher-resolution games.

## ROM Sources

- [chip8-archive](https://github.com/JohnEarnest/chip8-archive) — public domain ROMs
- [chip8-roms](https://github.com/kripod/chip8-roms) — test suites and games
- [chip8-test-suite](https://github.com/Timendus/chip8-test-suite) — opcode compliance tests

## Stack

- **C** (v1) — concise, close to the metal
- **Rust** (v2) — memory safety, nicer debugger UI
- **SDL2** — window, event loop, audio beep, keyboard input

## How to Build

```bash
cd chip8-emu
# C + SDL2
make
./chip8-emu roms/pong.ch8

# Rust (v2)
cargo build --release
cargo run -- roms/pong.ch8
```

## Controls

CHIP-8 uses a 4×4 hex keypad. Mapped to keyboard:

```
1 2 3 4     →     1 2 3 C
Q W E R     →     4 5 6 D
A S D F     →     7 8 9 E
Z X C V     →     A 0 B F
```

| Emulator Key | CHIP-8 Key |
|-------------|------------|
| `1`–`4` | `1`–`4` |
| `Q`–`R` | `4`–`D` |
| `A`–`F` | `7`–`E` |
| `Z`–`V` | `A`–`F` |

## License

Apache-2.0. ROMs are not included; download from the sources above.

---

## ☕ Support the Developer

If this project saved you time, solved a problem, or just made your day a little more neon, you can fuel the next one:

[![Buy Me A Coffee](https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png)](https://buymeacoffee.com/synthalorian)
