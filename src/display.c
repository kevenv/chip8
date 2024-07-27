/*
 * Copyright (c) 2024, Keven Villeneuve.
 *
 * SPDX-License-Identifier: MIT License
 */

#include "display.h"

#include <string.h> // memset

void display_reset(display_t* display)
{
    display_clear(display);
    for (u32 i = 0; i < VRAM_SIZE; i++) {
        u32 x = i % DISPLAY_W;
        u32 y = i / DISPLAY_W;
        if (y % 2 == 0) {
            display->vram[i] = (x + 1) % 2;
        }
        else {
            display->vram[i] = x % 2;
        }
    }
    display->font_rom = font;
}

void display_clear(display_t* display)
{
    memset(display->vram, 0, VRAM_SIZE);
}
