/*
 * Copyright (c) 2024, Keven Villeneuve.
 *
 * SPDX-License-Identifier: MIT License
 */

#include "speaker.h"

void speaker_reset(speaker_t* speaker)
{
    speaker->state = SPEAKER_RESET;
}

void speaker_tick(speaker_t* speaker, u8 ST)
{
    if (speaker->state == SPEAKER_RESET && ST > 0) {
        speaker->state = SPEAKER_START;
    }
    else if(speaker->state == SPEAKER_PLAYING && ST == 0) {
        speaker->state = SPEAKER_STOP;
    }
}
