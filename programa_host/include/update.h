#pragma once

#include "ringbuffer.h"
#include "screen.h"

typedef enum {
    TS_2_5MS = 2500,
    TS_5MS = 5000,
    TS_10MS = 10000,
    TS_100MS = 100000,
    TS_1S = 1000000,
    TS_10S = 10000000,
} working_mode_t;

extern working_mode_t time_scale; // MICROSEGUNDOS

void update(RingBuffer *);

void mode_0(RingBuffer *);
void mode_1(RingBuffer *);