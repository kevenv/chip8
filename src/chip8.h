/*
 * Copyright (c) 2024, Keven Villeneuve.
 *
 * SPDX-License-Identifier: MIT License
 */

#pragma once

#include "types.h"

#define RAM_SIZE   4*1024 // 4 KB
#define STACK_SIZE 16*2 // 32 B

typedef struct keypad_t keypad_t;
typedef struct rom_t rom_t;

typedef struct chip8_t chip8_t;
struct chip8_t {
    // CPU
    u16 PC; // program counter
    u8 SP; // stack pointer
    u16 I; // index register
    u8 V[16]; // data registers

    u16 stack[STACK_SIZE/sizeof(u16)];
    u8 ram[RAM_SIZE];

    keypad_t* keypad;
};

void chip8_reset(chip8_t* chip8);
void chip8_load_rom(chip8_t* chip8, rom_t* rom);
void chip8_tick(chip8_t* chip8);
