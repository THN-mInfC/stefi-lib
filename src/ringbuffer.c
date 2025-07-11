#include "libstefi/ringbuffer.h"

#include <stdint.h>

void rb_init(rb_t *ringbuffer, uint8_t *buf, uint16_t capacity) {
    ringbuffer->buf = buf;
    ringbuffer->head = 0;
    ringbuffer->tail = 0;
    ringbuffer->capacity = capacity;
}

bool rb_is_empty(rb_t *ringbuffer) {
    bool is_empty = (ringbuffer->head == ringbuffer->tail);
    return is_empty;
}

bool rb_is_full(rb_t *ringbuffer) {
    bool is_full = ((ringbuffer->tail + 1)%ringbuffer->capacity == ringbuffer->head);
    return is_full;
}