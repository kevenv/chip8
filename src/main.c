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
    chip8_init(&chip8);
    chip8_load_rom(&chip8, &rom);
    chip8_run(&chip8);
    return 0;

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

    // events loop
    bool running = true;
    SDL_Event event;
    while (running && SDL_WaitEvent(&event)) {
        // handle events
        if (event.type == SDL_QUIT) {
            running = false;
        }
        
        // repaint
        SDL_FillRect(window_surface, NULL, SDL_MapRGBA(window_surface->format, 0, 0, 0, 255));
        SDL_UpdateWindowSurface(window);
    }

    // cleanup
    rom_free(&rom);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
