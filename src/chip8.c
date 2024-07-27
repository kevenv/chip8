/*
 * Copyright (c) 2024, Keven Villeneuve.
 *
 * SPDX-License-Identifier: MIT License
 */

#include "chip8.h"

#include <stdio.h> // printf
#include <stdlib.h> // rand
#include <string.h> // memcpy, memset
#include <time.h> // time
#include "debug.h"
#include "display.h"
#include "keypad.h"
#include "rom.h"

#define PC    chip8->PC
#define SP    chip8->SP
#define I     chip8->I
#define V     chip8->V
#define STACK chip8->stack
#define RAM   chip8->ram
#define DT    chip8->DT
#define ST    chip8->ST

#define VF    V[0xF] // VF is the FLAG register

void chip8_reset(chip8_t* chip8)
{
    PC = 0x0200;
    SP = 0;
    I = 0x0000;
    memset(V, 0, N_REGS);
    memset(STACK, 0, STACK_SIZE);
    memset(RAM, 0, RAM_SIZE);
    ST = 0;
    DT = 0;
    //srand(time(NULL));
    srand(123456789); // TODO:
}

void chip8_load_rom(chip8_t* chip8, rom_t* rom)
{
    memcpy(&RAM[0x0000], chip8->display->font_rom, FONT_ROM_SIZE);
    memcpy(&RAM[PC], rom->bytes, rom->size);
}

bool chip8_tick(chip8_t* chip8)
{
    // fetch
    u16 op = ((u16)RAM[PC + 0] << 8) | ((u16)RAM[PC + 1] << 0);
    PC += 2;

    // decode
    u8 op1 = (u8)((op >> 12) & 0xF);
    u8 op2 = (u8)(op & 0x00FF);
    u8 x = (u8)((op >> 8) & 0xF);
    u8 y = (u8)((op >> 4) & 0xF);
    u8 n = (u8)(op & 0x000F);
    u8 nn = (u8)(op & 0x00FF);
    u16 nnn = (u16)(op & 0x0FFF);
    printf("%X\n", op);
    dump_instruction(op);

    // execute
    switch (op1) {
        case 0x0:
            switch(op2) {
                case 0xE0: // 00E0
                    display_clear(chip8->display);
                    break;
                case 0xEE: // 00EE
                    PC = STACK[--SP];
                    break;
                default:
                    return false;
            }
            break;
        case 0x1: // 1NNN
            PC = nnn;
            break;
        case 0x2: // 2NNN
            STACK[SP++] = PC;
            PC = nnn;
            break;
        case 0x3: // 3XNN
            PC = (V[x] == nn) ? PC+2 : PC;
            break;
        case 0x4: // 4XNN
            PC = (V[x] != nn) ? PC+2 : PC;
            break;
        case 0x5: // 5XY0
            PC = (V[x] == V[y]) ? PC+2 : PC;
            break;
        case 0x6: // 6XNN
            V[x] = nn;
            break;
        case 0x7: // 7XNN
            V[x] = V[x] + nn;
            break;
        case 0x8:
            switch (op2 & 0xF) {
                case 0x0: // 8XY0
                    V[x] = V[y];
                    break;
                case 0x1: // 8XY1
                    V[x] = V[x] | V[y];
                    break;
                case 0x2: // 8XY2
                    V[x] = V[x] & V[y];
                    break;
                case 0x3: // 8XY3
                    V[x] = V[x] ^ V[y];
                    break;
                case 0x4: // 8XY4
                    VF = V[x] + V[y] > 0xFF;
                    V[x] = V[x] + V[y];
                    break;
                case 0x5: // 8XY5
                    VF = V[x] > V[y];
                    V[x] = V[x] - V[y];
                    break;
                case 0x6: // 8XY6
                    VF = V[x] & 0x1;
                    V[x] = V[x] >> 1;
                    break;
                case 0x7: // 8XY7
                    VF = V[y] > V[x];
                    V[x] = V[y] - V[x];
                    break;
                case 0xE: // 8XYE
                    VF = (V[x] & 0b10000000) != 0;
                    V[x] = V[x] << 1;
                    break;
            }
            break;
        case 0x9: // 9XY0
            PC = (V[x] != V[y]) ? PC+2 : PC;
            break;
        case 0xA: // ANNN
            I = nnn;
            break;
        case 0xB: // BNNN
            PC = nnn + V[0];
            break;
        case 0xC: // CXNN
            u8 rnd = (u8)(rand() % 256);
            V[x] = rnd & nn;
            break;
        case 0xD: // DXYN
            // TODO:
            // n = 5;
            // I = 0x0000 + 8*FONT_SIZE;
            u32 w = SPRITE_W;
            u32 h = n; // n = sprite size = w*h/8 (1 bpp)
            VF = 0;
            for (u32 j = 0; j < h; j++) {
                u8 row = RAM[I + j];
                for (u32 i = 0; i < w; i++) {
                    u8 px = (row >> (w-1 - i)) & 0x1;
                    u8* px_vram = &chip8->display->vram[(x + i) + (y + j)*DISPLAY_W];
                    if ((px ^ *px_vram) == 0) {
                        VF = 1; // pixel collision
                    }
                    *px_vram = *px_vram ^ px; // XOR draw
                }
            }
            break;
        case 0xE:
            switch (op2) {
                case 0x9E: // EX9E
                    //PC = (chip8->keypad.keys[V[x]]) ? PC+2 : PC;
                    PC = keypad_pressed(chip8->keypad, V[x]) ? PC+2 : PC;
                    break;
                case 0xA1: // EXA1
                    PC = !keypad_pressed(chip8->keypad, V[x]) ? PC+2 : PC;
                    break;
            }
            break;
        case 0xF:
            switch (op2) {
                case 0x07: // FX07
                    V[x] = DT;
                    break;
                case 0x0A: // FX0A
                    u8 key;
                    if (keypad_any_pressed(chip8->keypad, &key)) {
                        V[x] = key;
                        PC = PC+2; // skip next instruction
                    }
                    else {
                        PC = PC-2; // wait until pressed
                    }
                    break;
                case 0x15: // FX15
                    DT = V[x];
                    break;
                case 0x18: // FX18
                    // -
                    ST = V[x];
                    break;
                case 0x1E: // FX1E
                    I = I + V[x];
                    break;
                case 0x29: // FX29
                    I = 0x0000 + V[x] * FONT_SIZE; // address of font sprite V[x]
                    break;
                case 0x33:
                    // TODO:
                    printf("?\n");
                    break;
                case 0x55: // FX55
                    memcpy(&RAM[I], V, x+1);
                    break;
                case 0x65: // FX65
                    memcpy(V, &RAM[I], x+1);
                    break;
            }
            break;
    }

    // update timers
    if (DT > 0) DT--;
    if (DT > 0) ST--;
    
    return true;
}
