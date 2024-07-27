/*
 * Copyright (c) 2024, Keven Villeneuve.
 *
 * SPDX-License-Identifier: MIT License
 */

#pragma once

#include <stdio.h> // printf
#include "types.h"

static void dump_instruction(u16 instruction)
{
    u8 op1 = (u8)((instruction >> 12) & 0xF);
    u8 op2 = (u8)(instruction & 0x00FF);

    switch (op1) {
        case 0:
            switch(op2) {
                case 0xE0: printf("CLS\n"); break; // 00E0
                case 0xEE: printf("RET\n"); break; // 00EE
            }
            break;
        case 0x1: printf("JP nnn\n"); break; // 1NNN
        case 0x2: printf("CALL nnn\n"); break; // 2NNN
        case 0x3: printf("SE Vx, nn\n"); break; // 3XNN
        case 0x4: printf("SNE Vx, nn\n"); break; // 4XNN
        case 0x5: printf("SE Vx, Vy\n"); break; // 5XY0
        case 0x6: printf("LD Vx, nn\n"); break; // 6XNN
        case 0x7: printf("ADD Vx, nn\n"); break; // 7XNN
        case 0x8:
            switch (op2 & 0xF) {
                case 0x0: printf("LD Vx, Vy\n"); break; // 8XY0
                case 0x1: printf("OR Vx, Vy\n"); break; // 8XY1
                case 0x2: printf("AND Vx, Vy\n"); break; // 8XY2
                case 0x3: printf("XOR Vx, Vy\n"); break; // 8XY3
                case 0x4: printf("ADD Vx, Vy\n"); break; // 8XY4
                case 0x5: printf("SUB Vx, Vy\n"); break; // 8XY5
                case 0x6: printf("SHR Vx\n"); break; // 8XY6
                case 0x7: printf("SUBN Vx, Vy\n"); break; // 8XY7
                case 0xE: printf("SHL Vx\n"); break; // 8XYE
            }
            break;
        case 0x9: printf("SNE Vx, Vy\n"); break; // 9XY0
        case 0xA: printf("LD I, nnn\n"); break; // ANNN
        case 0xB: printf("JP V0, nnn\n"); break; // BNNN
        case 0xC: break;
        case 0xD: break;
        case 0xE:
            switch (op2) {
                case 0x9E: printf("SKP Vx\n"); break; // EX9E
                case 0xA1: printf("SKNP Vx\n"); break; // EXA1
            }
            break;
        case 0xF:
            switch (op2) {
                case 0x07: break;
                case 0x0A: printf("LD Vx, K\n"); break; // FX0A
                case 0x15: break;
                case 0x18: break;
                case 0x1E: printf("ADD I, Vx\n"); break; // FX1E
                case 0x29: printf("LD F, Vx\n"); break; // FX29
                case 0x33: break;
                case 0x55: printf("LD [I], Vx\n"); break; // FX55
                case 0x65: printf("LD Vx, [I]\n"); break; // FX65
            }
            break;
    }
}
