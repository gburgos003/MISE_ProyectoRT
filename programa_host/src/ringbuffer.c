#include "ringbuffer.h"

RingBuffer data_buffer;

RingBuffer create_ring_buffer() {
    RingBuffer buf;

    buf.data = (uint16_t *) malloc(RING_BUFFER_SIZE * sizeof(uint16_t));
    buf.write_index = 0;
    buf.read_index = 0;

    return buf;
}

int pop_ring_buffer(RingBuffer * buffer, uint16_t * data_ptr) {
    int next;

    if(buffer->read_index == buffer->write_index) {
        return -1;
    }

    next = buffer->read_index + 1;
    if (next >= RING_BUFFER_SIZE) {
        next = 0;
    }

    *data_ptr = buffer->data[buffer->read_index];
    buffer->read_index = next;
    return 0;
}

int push_ring_buffer(RingBuffer * buffer, uint16_t data_value) {
    int next;

    next = buffer->write_index + 1;
    if (next >= RING_BUFFER_SIZE) {
        next = 0;
    }

    if(next == buffer->read_index) {
        return -1;
    }

    buffer->data[buffer->write_index] = data_value;
    buffer->write_index = next;
    return 0;
}

void vaciar_ring_buffer(RingBuffer * buffer) {
    buffer->read_index = 0;
    buffer->write_index = 0;
}