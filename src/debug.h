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
    // TODO: decode_instruction() ?
    u8 op1 = (u8)((instruction >> 12) & 0xF);
    u8 op2 = (u8)(instruction & 0x00FF);
    u8 x = (u8)((instruction >> 8) & 0xF);
    u8 y = (u8)((instruction >> 4) & 0xF);
    u8 n = (u8)(instruction & 0x000F);
    u8 nn = (u8)(instruction & 0x00FF);
    u16 nnn = (u16)(instruction & 0x0FFF);

    switch (op1) {
        case 0:
            switch(op2) {
                case 0xE0: printf("CLS\n"); break; // 00E0
                case 0xEE: printf("RET\n"); break; // 00EE
                default: printf("STOP\n"); break;
            }
            break;
        case 0x1: printf("JP 0x%hX\n", nnn); break; // 1NNN
        case 0x2: printf("CALL 0x%hX\n", nnn); break; // 2NNN
        case 0x3: printf("SE V%hhX, %hhu\n", x, nn); break; // 3XNN
        case 0x4: printf("SNE V%hhX, %hhu\n", x, nn); break; // 4XNN
        case 0x5: printf("SE V%hhX, V%hhX\n", x, y); break; // 5XY0
        case 0x6: printf("LD V%hhX, %hhu\n", x, nn); break; // 6XNN
        case 0x7: printf("ADD V%hhX, %hhu\n", x, nn); break; // 7XNN
        case 0x8:
            switch (op2 & 0xF) {
                case 0x0: printf("LD V%hhX, V%hhX\n", x, y); break; // 8XY0
                case 0x1: printf("OR V%hhX, V%hhX\n", x, y); break; // 8XY1
                case 0x2: printf("AND V%hhX, V%hhX\n", x, y); break; // 8XY2
                case 0x3: printf("XOR V%hhX, V%hhX\n", x, y); break; // 8XY3
                case 0x4: printf("ADD V%hhX, V%hhX\n", x, y); break; // 8XY4
                case 0x5: printf("SUB V%hhX, V%hhX\n", x, y); break; // 8XY5
                case 0x6: printf("SHR V%hhX\n", x); break; // 8XY6
                case 0x7: printf("SUBN V%hhX, V%hhX\n", x, y); break; // 8XY7
                case 0xE: printf("SHL V%hhX\n", x); break; // 8XYE
            }
            break;
        case 0x9: printf("SNE V%hhX, V%hhX\n", x, y); break; // 9XY0
        case 0xA: printf("LD I, 0x%hX\n", nnn); break; // ANNN
        case 0xB: printf("JP V0, 0x%hX\n", nnn); break; // BNNN
        case 0xC: printf("RND V%hhX, %hhu\n", x, nn); break; // CXNN
        case 0xD: printf("DRW V%hhX, V%hhX, n\n", x, y); break; // DXYN
        case 0xE:
            switch (op2) {
                case 0x9E: printf("SKP V%hhX\n", x); break; // EX9E
                case 0xA1: printf("SKNP V%hhX\n", x); break; // EXA1
            }
            break;
        case 0xF:
            switch (op2) {
                case 0x07: printf("LD V%hhX, DT\n", x); break; // FX07
                case 0x0A: printf("LD V%hhX, K\n", x); break; // FX0A
                case 0x15: printf("LD DT, V%hhX\n", x); break; // FX15
                case 0x18: printf("LD ST, V%hhX\n", x); break; // FX18
                case 0x1E: printf("ADD I, V%hhX\n", x); break; // FX1E
                case 0x29: printf("LDF V%hhX\n", x); break; // FX29
                case 0x33: printf("unimplemented\n"); break;
                case 0x55: printf("LD [I], V%hhX\n", x); break; // FX55
                case 0x65: printf("LD V%hhX, [I]\n", x); break; // FX65
            }
            break;
    }
}
