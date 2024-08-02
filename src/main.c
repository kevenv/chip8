/*
 * Copyright (c) 2024, Keven Villeneuve.
 *
 * SPDX-License-Identifier: MIT License
 */

#include <stdio.h> // printf
#include <stdlib.h> // exit_code
#include "app.h"
#include "chip8.h"
#include "display.h"
#include "keypad.h"
#include "rom.h"
#include "speaker.h"
#include "types.h"

/*
    Main

    The emulator entry point and main updates loop.
*/

int main(int argc, char* argv[])
{
    // parse args
    if (argc != 2) {
        fprintf(stderr, "ERROR: usage should be ./chip8 [rom.ch8]\n");
        return EXIT_FAILURE;
    }

    // open window
    app_t app;
    if (!app_init(&app)) {
        return EXIT_FAILURE;
    }
    
    // load ROM
    rom_t rom;
    if (!rom_load(&rom, argv[1])) {
        return EXIT_FAILURE;
    }

    // init chip8
    chip8_t chip8;
    display_t display;
    keypad_t keypad;
    speaker_t speaker;
    chip8.display = &display;
    chip8.keypad = &keypad;
    display_reset(&display);
    keypad_reset(&keypad);
    speaker_reset(&speaker);
    chip8_reset(&chip8);
    chip8_load_rom(&chip8, &rom);

    // tick emulator
    u32 cycles = 0;
    bool running = true;
    while (running) {
        // tick @ 500Hz
        if (!chip8_tick(&chip8)) {
            running = false;
            printf("QUIT\n");
            break;
        }
        
        // tick @ 60Hz
        if (cycles >= CPU_FREQ_HZ / TIMER_FREQ_HZ) {
            cycles = 0;

            if (!app_update(&app, &keypad)) {
                running = false;
                break;
            }
            chip8_timers_tick(&chip8);
            speaker_tick(&speaker, chip8.ST);
            // printf("%c", chip8.ST > 0 ? '*' : '-');
            if (speaker.state == SPEAKER_START) {
                // printf("S");
                speaker.state = SPEAKER_PLAYING;
            }
            if (speaker.state == SPEAKER_STOP) {
                // printf("E");
                speaker.state = SPEAKER_RESET;
            }

            app_render(&app, &display);
        }

        app_sleep(&app, (u32)(1.0f/CPU_FREQ_HZ*1000));
        cycles++;
    }

    // cleanup
    rom_free(&rom);
    app_close(&app);

    return EXIT_SUCCESS;
}
