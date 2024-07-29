/*
 * Copyright (c) 2024, Keven Villeneuve.
 *
 * SPDX-License-Identifier: MIT License
 */

#pragma once

#include "types.h"

#define N_REGS     16
#define RAM_SIZE   4096 // 4 KB
#define STACK_SIZE 32 // 32 B

typedef struct display_t display_t;
typedef struct keypad_t keypad_t;
typedef struct rom_t rom_t;

typedef struct chip8_t chip8_t;
struct chip8_t {
    // CPU
    u16 PC; // program counter
    u8 SP; // stack pointer
    u16 I; // index register
    u8 V[N_REGS]; // data registers

    // memory
    u16 stack[STACK_SIZE/sizeof(u16)];
    u8 ram[RAM_SIZE];

    // timers
    u8 DT; // delay timer
    u8 ST; // sound timer

    // devices
    display_t* display;
    keypad_t* keypad;
};

void chip8_reset(chip8_t* chip8);
void chip8_load_rom(chip8_t* chip8, rom_t* rom);
bool chip8_tick(chip8_t* chip8);
void chip8_timers_tick(chip8_t* chip8);

void chip8_dxyn(chip8_t* chip8, u8 x, u8 y, u8 n);
