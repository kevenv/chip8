/*
 * Copyright (c) 2024, Keven Villeneuve.
 *
 * SPDX-License-Identifier: MIT License
 */

#pragma once

#include "types.h"

/*
    ROM

    Binary dump of a user program.
*/

typedef struct rom_t rom_t;
struct rom_t {
    u8* bytes;
    u32 size;
};

bool rom_load(rom_t* rom, const char* file_path);
void rom_free(rom_t* rom);
