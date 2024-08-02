/*
 * Copyright (c) 2024, Keven Villeneuve.
 *
 * SPDX-License-Identifier: MIT License
 */

#include "app.h"

#include <stdio.h> // printf
#include "keypad.h"

bool app_init(app_t* app)
{
    // init SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "ERROR: could not initialize SDL: %s\n", SDL_GetError());
        // goto sdl_init_fail;
    }
    app->window = SDL_CreateWindow("Chip8",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_W, WINDOW_H,
        SDL_WINDOW_SHOWN
    );
    if (!app->window) {
        fprintf(stderr, "ERROR: could not create SDL window: %s\n", SDL_GetError());
        // goto sdl_window_fail;
    }
    app->window_surface = SDL_GetWindowSurface(app->window);
    return true;
}

void app_close(app_t* app)
{
    SDL_DestroyWindow(app->window);
    SDL_Quit();
}

bool app_update(app_t* app, keypad_t* keypad)
{
    // handle events
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            return false;
        }
    }

    // update keypad
    const u8* keys = SDL_GetKeyboardState(NULL);
    keypad->keys[0] = keys[SDL_SCANCODE_X];
    keypad->keys[1] = keys[SDL_SCANCODE_1];
    keypad->keys[2] = keys[SDL_SCANCODE_2];
    keypad->keys[3] = keys[SDL_SCANCODE_3];
    keypad->keys[4] = keys[SDL_SCANCODE_Q];
    keypad->keys[5] = keys[SDL_SCANCODE_W];
    keypad->keys[6] = keys[SDL_SCANCODE_E];
    keypad->keys[7] = keys[SDL_SCANCODE_A];
    keypad->keys[8] = keys[SDL_SCANCODE_S];
    keypad->keys[9] = keys[SDL_SCANCODE_D];
    keypad->keys[10] = keys[SDL_SCANCODE_Z];
    keypad->keys[11] = keys[SDL_SCANCODE_C];
    keypad->keys[12] = keys[SDL_SCANCODE_4];
    keypad->keys[13] = keys[SDL_SCANCODE_R];
    keypad->keys[14] = keys[SDL_SCANCODE_F];
    keypad->keys[15] = keys[SDL_SCANCODE_V];
    return true;
}

void app_render(app_t* app, display_t* display)
{
    // copy the framebuffer of the Chip8 to the SDL window
    SDL_LockSurface(app->window_surface);
    for (u32 i = 0; i < WINDOW_W * WINDOW_H; i++) {
        u32 x = (i % WINDOW_W) / WINDOW_SCALE;
        u32 y = (i / WINDOW_W) / WINDOW_SCALE;
        u8 px = display->vram[x + y * DISPLAY_W] ? 255 : 0;
        u8* pixels = app->window_surface->pixels;
        pixels[i*4 + 0] = px;
        pixels[i*4 + 1] = px;
        pixels[i*4 + 2] = px;
        pixels[i*4 + 3] = px;
    }
    SDL_UnlockSurface(app->window_surface);
    SDL_UpdateWindowSurface(app->window);
}

void app_sleep(app_t* app, u32 sleep_ms)
{
    SDL_Delay(sleep_ms);
}
