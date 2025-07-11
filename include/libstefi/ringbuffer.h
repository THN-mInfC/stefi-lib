#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef struct ringbuffer rb_t;

/* Implementation: Leave one slot empty, buffer hold capacity - 1 elements
 */
struct ringbuffer {
    volatile uint8_t *buf;
    uint16_t head; //read index
    uint16_t tail; //write index
    uint16_t capacity;
};

void rb_init(rb_t *ringbuffer, uint8_t *buf, uint16_t capacity);
bool rb_is_empty(rb_t *ringbuffer);
bool rb_is_full(rb_t *ringbuffer);

static inline void rb_insert(rb_t *ringbuffer, uint8_t elem) {
    uint16_t tail = ringbuffer->tail;
    ringbuffer->buf[tail] = elem;
    ringbuffer->tail = (tail + 1 == ringbuffer->capacity) ? 0 : tail + 1;
}

static inline uint8_t rb_remove(rb_t *ringbuffer) {
    uint16_t head = ringbuffer->head;
    uint8_t elem = ringbuffer->buf[head];
    ringbuffer->head = (head + 1 == ringbuffer->capacity) ? 0 : head + 1;
    return elem;
}

