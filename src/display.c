/*
 * Copyright (c) 2024, Keven Villeneuve.
 *
 * SPDX-License-Identifier: MIT License
 */

#include "display.h"

void display_reset(display_t* display)
{
    display->font_rom = font;
}
