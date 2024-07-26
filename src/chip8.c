/*
 * Copyright (c) 2024, Keven Villeneuve.
 *
 * SPDX-License-Identifier: MIT License
 */

#include "chip8.h"

#include <stdio.h> // printf
#include <string.h> // memcpy, memset
#include "rom.h"

#define PC    chip8->PC
#define SP    chip8->SP
#define I     chip8->I
#define V     chip8->V
#define STACK chip8->stack
#define RAM   chip8->ram

#define VF    V[0xF] // VF is the FLAG register

void chip8_init(chip8_t* chip8)
{
    PC = 0x0200;
    SP = 0;
    I = 0x0000;
    memset(V, 0, 16);
    memset(RAM, 0, RAM_SIZE);
}

void chip8_load_rom(chip8_t* chip8, rom_t* rom)
{
    memcpy(&RAM[PC], rom->bytes, rom->size);
}

void chip8_run(chip8_t* chip8)
{
    while (1) {
        // fetch
        // 2 byte long instruction
		// big-endian
        u16 op = 
        ((u16)RAM[PC + 0] << 8) |
        ((u16)RAM[PC + 1] << 0);
        PC += 2;

        // decode
        u8 op1 = (u8)((op >> 12) & 0xF);
        u8 op2 = (u8)(op & 0x00FF);
        u8 x = (u8)((op >> 8) & 0xF);
        u8 y = (u8)((op >> 4) & 0xF);
        u8 nn = (u8)(op & 0x00FF);
        u16 nnn = (u8)(op & 0x0FFF);

        // execute
        printf("%X\n", op);
        switch (op1) {
            case 0:
                switch(op2) {
                    case 0xE0: // 00E0
                        // TODO:
                        break;
                    case 0xEE: // 00EE
                        printf("RET\n");
                        PC = STACK[SP--];
                        break;
                }
                break;
            case 0x1: // 1NNN
                printf("JP nnn\n");
                PC = nnn;
                break;
            case 0x2: // 2NNN
                printf("CALL nnn\n");
                STACK[SP++] = PC;
                PC = nnn;
                break;
            case 0x3: // 3XNN
                printf("SE Vx, nn\n");
                if (V[x] == nn) 
                    PC += 2;
                break;
            case 0x4: // 4XNN
                printf("SNE Vx, nn\n");
                if (V[x] != nn)
                    PC += 2;
                break;
            case 0x5: // 5XY0
                printf("SE Vx, Vy\n");
                if (V[x] == V[y])
                    PC += 2;
                break;
            case 0x6: // 6XNN
                printf("LD Vx, nn\n");
                V[x] = nn;
                break;
            case 0x7: // 7XNN
                printf("ADD Vx, nn\n");
                V[x] += nn;
                break;
            case 0x8:
                switch (op2 & 0xF) {
                    case 0x0: // 8XY0
                        printf("LD Vx, Vy\n");
                        V[x] = V[y];
                        break;
                    case 0x1: // 8XY1
                        printf("OR Vx, Vy\n");
                        V[x] = V[x] | V[y];
                        break;
                    case 0x2: // 8XY2
                        printf("AND Vx, Vy\n");
                        V[x] = V[x] & V[y];
                        break;
                    case 0x3: // 8XY3
                        printf("XOR Vx, Vy\n");
                        V[x] = V[x] ^ V[y];
                        break;
                    case 0x4: // 8XY4
                        printf("ADD Vx, Vy\n");
                        VF = V[x] + V[y] > 0xFF;
                        V[x] = V[x] + V[y];
                        break;
                    case 0x5: // 8XY5
                        printf("SUB Vx, Vy\n");
                        VF = V[x] > V[y];
                        V[x] = V[x] - V[y];
                        break;
                    case 0x6: // 8XY6
                        printf("SHR Vx\n");
                        VF = V[x] & 0x1;
                        V[x] = V[x] >> 1;
                        break;
                    case 0x7: // 8XY7
                        printf("SUBN Vx, Vy\n");
                        VF = V[y] > V[x];
                        V[x] = V[y] - V[x];
                        break;
                    case 0xE: // 8XYE
                        printf("SHL Vx\n");
                        VF = (V[x] & 0b10000000) != 0;
                        V[x] = V[x] << 1;
                        break;
                }
                break;
            case 0x9: // 9XY0
                printf("SNE Vx, Vy\n");
                if (V[x] != V[y])
                    PC += 2;
                break;
            case 0xA: // ANNN
                printf("LD I, nnn\n");
                I = nnn;
                break;
            case 0xB: // BNNN
                printf("JP V0, nnn\n");
                PC = nnn + V[0];
                break;
            case 0xC:
                // TODO:
                break;
            case 0xD:
                // TODO:
                break;
            case 0xE:
                switch (op2) {
                    case 0x9E:
                        break;
                    case 0xA1:
                        break;
                }
                break;
            case 0xF:
                switch (op2) {
                    case 0x07:
                        // TODO:
                        break;
                    case 0x0A:
                        // TODO:
                        break;
                    case 0x15:
                        // TODO:
                        break;
                    case 0x18:
                        // TODO:
                        break;
                    case 0x1E: // FX1E
                        printf("ADD I, Vx");
                        I = I + V[x];
                        break;
                    case 0x29:
                        // TODO:
                        break;
                    case 0x33:
                        // TODO:
                        break;
                    case 0x55: // FX55
                        printf("LD [I], Vx\n");
                        for (u32 i = 0; i < x; i++) {
                            RAM[I + i] = V[i]; 
                        }
                        break;
                    case 0x65: // FX65
                        printf("LD Vx, [I]\n");
                        for (u32 i = 0; i < x; i++) {
                            V[i] = RAM[I + i];
                        }
                        break;
                }
                break;
        }
    }
}
