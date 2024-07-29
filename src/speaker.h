/*
 * Copyright (c) 2024, Keven Villeneuve.
 *
 * SPDX-License-Identifier: MIT License
 */

#include "types.h"

/*
    Speaker

    only one tone (1-bit audio)
    ON when ST > 0
*/

// RESET -> START -> PLAYING -> STOP -> RESET
typedef enum speaker_state_t speaker_state_t;
enum speaker_state_t {
    SPEAKER_RESET,
    SPEAKER_START,
    SPEAKER_PLAYING,
    SPEAKER_STOP
};

typedef struct speaker_t speaker_t;
struct speaker_t {
    speaker_state_t state;
};

void speaker_reset(speaker_t* speaker);
void speaker_tick(speaker_t* speaker, u8 ST);
