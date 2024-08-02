/*
 * Copyright (c) 2024, Keven Villeneuve.
 *
 * SPDX-License-Identifier: MIT License
 */

#pragma once

#include "types.h"

/*
    Keypad

    A keypad of 16 keys indexed from 0 to F.

    Layout:
        1 2 3 C
        4 5 6 D
        7 8 9 E
        A 0 B F

    Arrow keys: 
          2
        4   6
          8
*/

#define N_KEYS 16

typedef struct keypad_t keypad_t;
struct keypad_t {
    bool keys[N_KEYS];
};

void keypad_reset(keypad_t* keypad);
bool keypad_pressed(keypad_t* keypad, u8 key);
bool keypad_any_pressed(keypad_t* keypad, u8* key);
