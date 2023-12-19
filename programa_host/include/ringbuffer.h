#pragma once

#include <stdint.h>
#include <stdlib.h>

#define RING_BUFFER_SIZE 1000000

typedef struct RingBuffer {
    uint32_t * data;
    int write_index;
    int read_index;
} RingBuffer;

RingBuffer create_ring_buffer(void);
int pop_ring_buffer(RingBuffer *, uint32_t *);
int push_ring_buffer(RingBuffer *, uint32_t);
