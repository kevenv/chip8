/*
 * Copyright (c) 2024, Keven Villeneuve.
 *
 * SPDX-License-Identifier: MIT License
 */

#include "rom.h"

#include <stdio.h> // printf, fopen
#include <stdlib.h> // malloc

bool rom_load(rom_t* rom, const char* file_path)
{
    FILE* file = fopen(file_path, "rb");
    if (!file) {
        fprintf(stderr, "ERROR: cannot open file '%s'\n", file_path);
        return false;
    }

    printf("loading ROM '%s'\n", file_path);

    // get file_size
    fseek(file, 0, SEEK_END);
    rom->size = (u32)ftell(file);
    fseek(file, 0, SEEK_SET);

    // read ROM
    rom->bytes = (u8*)malloc(rom->size);
    fread(rom->bytes, rom->size, 1, file);

    fclose(file);
    return true;
}

void rom_free(rom_t* rom)
{
    free(rom->bytes);
}
