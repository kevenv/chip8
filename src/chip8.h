/*
 * Copyright (c) 2024, Keven Villeneuve.
 *
 * SPDX-License-Identifier: MIT License
 */

#pragma once

#include "types.h"

#define RAM_SIZE 4*1024
#define STACK_SIZE 12

// VF is the FLAG register
#define VF chip8->V[0x0F]

typedef struct rom_t rom_t;

typedef struct chip8_t chip8_t;
struct chip8_t {
    // CPU
    u16 PC; // program counter
    u8 SP; // stack pointer
    u16 I; // index register
    u8 V[16]; // data registers

    // Stack
    u16 stack[STACK_SIZE]; // only for subroutine return address
    
    // RAM
    u8 RAM[RAM_SIZE];
};

void chip8_init(chip8_t* chip8);
void chip8_load_rom(chip8_t* chip8, rom_t* rom);
void chip8_run(chip8_t* chip8);
