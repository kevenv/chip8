/*
 * Copyright (c) 2024, Keven Villeneuve.
 *
 * SPDX-License-Identifier: MIT License
 */

#include "keypad.h"

#include <string.h> // memset

void keypad_reset(keypad_t* keypad)
{
    memset(keypad->keys, false, N_KEYS*sizeof(bool));
}

bool keypad_pressed(keypad_t* keypad, u8 key)
{
    return keypad->keys[key];
}

bool keypad_any_pressed(keypad_t* keypad, u8* key)
{
    for (u8 i = 0; i < N_KEYS; i++) {
        if (keypad->keys[i]) {
            *key = i;
            return true;
        }
    }
    return false;
}
