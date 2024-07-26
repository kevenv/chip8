/*
 * Copyright (c) 2024, Keven Villeneuve.
 *
 * SPDX-License-Identifier: MIT License
 */

#include "chip8.h"

#include <string.h> // memcpy, memset
#include "rom.h"

void chip8_init(chip8_t* chip8)
{
    chip8->PC = 0x0200;
    chip8->SP = 0x00;
    chip8->I = 0x0000;
    memset(chip8->V, 0, 16);
    memset(chip8->RAM, 0, RAM_SIZE);
}

void chip8_load_rom(chip8_t* chip8, rom_t* rom)
{
    memcpy(chip8->RAM, rom->bytes, rom->size);   
}

void chip8_run(chip8_t* chip8)
{
    while (1) {
        // fetch

        // 2 byte long instruction
		// big-endian
        u16 inst = 
        ((u16)chip8->RAM[chip8->PC + 0] << 8) |
        ((u16)chip8->RAM[chip8->PC + 1] << 0);
        chip8->PC += 2;

        // decode
        switch (inst) {
            
        }

        // execute
    }
}
