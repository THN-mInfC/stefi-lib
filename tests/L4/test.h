#pragma once
#include <stdint.h>
#include <stdio.h>

typedef void (*test_fn_t)();

typedef struct {
    const char *test_name;
    test_fn_t test_fn;
} tests_t;

void run_all_tests(const tests_t *tests, int count);
