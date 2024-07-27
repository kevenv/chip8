/*
 * Copyright (c) 2024, Keven Villeneuve.
 *
 * SPDX-License-Identifier: MIT License
 */

#include <SDL2/SDL.h>
#include <stdbool.h> // bool
#include <stdio.h> // printf
#include <stdlib.h> // exit_code
#include "chip8.h"
#include "keypad.h"
#include "rom.h"

int main(int argc, char* argv[])
{
    // parse args
    if (argc != 2) {
        fprintf(stderr, "ERROR: usage should be ./chip8 [rom.ch8]\n");
        return EXIT_FAILURE;
    }
    
    // load ROM
    rom_t rom;
    if (!rom_load(&rom, argv[1])) {
        return EXIT_FAILURE;
    }

    // init chip8
    chip8_t chip8;
    keypad_t keypad;
    chip8.keypad = &keypad;
    keypad_reset(&keypad);
    chip8_reset(&chip8);
    chip8_load_rom(&chip8, &rom);

    // init SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "ERROR: could not initialize SDL: %s\n", SDL_GetError());
        // goto sdl_init_fail;
    }
    SDL_Window* window = SDL_CreateWindow("Chip8",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        fprintf(stderr, "ERROR: could not create SDL window: %s\n", SDL_GetError());
        // goto sdl_window_fail;
    }
    SDL_Surface* window_surface = SDL_GetWindowSurface(window);

    // main loop
    bool running = true;
    SDL_Event event;
    while (running) {
        // handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                break;
            }
        }

        // update
        const u8* keys = SDL_GetKeyboardState(NULL);
        keypad.keys[0] = keys[SDL_SCANCODE_1];
        keypad.keys[1] = keys[SDL_SCANCODE_2];
        keypad.keys[2] = keys[SDL_SCANCODE_3];
        keypad.keys[3] = keys[SDL_SCANCODE_4];
        keypad.keys[4] = keys[SDL_SCANCODE_Q];
        keypad.keys[5] = keys[SDL_SCANCODE_W];
        keypad.keys[6] = keys[SDL_SCANCODE_E];
        keypad.keys[7] = keys[SDL_SCANCODE_R];
        keypad.keys[8] = keys[SDL_SCANCODE_A];
        keypad.keys[9] = keys[SDL_SCANCODE_S];
        keypad.keys[10] = keys[SDL_SCANCODE_D];
        keypad.keys[11] = keys[SDL_SCANCODE_F];
        keypad.keys[12] = keys[SDL_SCANCODE_Z];
        keypad.keys[13] = keys[SDL_SCANCODE_X];
        keypad.keys[14] = keys[SDL_SCANCODE_C];
        keypad.keys[15] = keys[SDL_SCANCODE_V];

        chip8_tick(&chip8);
        
        // render
        SDL_FillRect(window_surface, NULL, SDL_MapRGBA(window_surface->format, 0, 0, 0, 255));

        SDL_UpdateWindowSurface(window);
        SDL_Delay(10);
    }

    // cleanup
    rom_free(&rom);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
