/*
 * Copyright (c) 2024, Keven Villeneuve.
 *
 * SPDX-License-Identifier: MIT License
 */

#pragma once

#include <SDL2/SDL.h>
#include "display.h"
#include "types.h"

#define WINDOW_SCALE 8
#define WINDOW_W     (DISPLAY_W * WINDOW_SCALE)
#define WINDOW_H     (DISPLAY_H * WINDOW_SCALE)

typedef struct keypad_t keypad_t;

/*
    App

    Interface between the Chip8 emulator and the SDL window,
    making the rest of the code independent of SDL.
    The emulator is hooked to the SDL window to implement I/O.
*/

typedef struct app_t app_t;
struct app_t {
    SDL_Window* window;
    SDL_Surface* window_surface;
};

bool app_init(app_t* app);
void app_close(app_t* app);
bool app_update(app_t* app, keypad_t* keypad);
void app_render(app_t* app, display_t* display);
void app_sleep(app_t* app, u32 sleep_ms);
