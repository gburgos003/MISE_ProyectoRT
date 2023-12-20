#pragma once

#include "ringbuffer.h"
#include "screen.h"

typedef enum {
    TS_100US = 100,
    TS_1MS = 1000,
    TS_10MS = 10000,
    TS_100MS = 100000,
    TS_1S = 1000000,
    TS_10S = 10000000,
} working_mode_t;

extern working_mode_t time_scale; // MICROSEGUNDOS

void update(RingBuffer *, double elapsed);

void mode_0(RingBuffer *, double elapsed);
void mode_1(RingBuffer *, double elapsed);