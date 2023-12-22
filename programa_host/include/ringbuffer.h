#pragma once

#include <stdint.h>
#include <stdlib.h>

#define RING_BUFFER_SIZE 500

typedef struct RingBuffer {
    uint16_t * data;
    int write_index;
    int read_index;
} RingBuffer;

RingBuffer create_ring_buffer(void);
int pop_ring_buffer(RingBuffer *, uint16_t *);
int push_ring_buffer(RingBuffer *, uint16_t);
