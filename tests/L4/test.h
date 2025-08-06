#pragma once
#include <stdint.h>
#include <stdio.h>

typedef void (*test_fn_t)();

typedef struct {
    const char *test_name;
    test_fn_t test_fn;
} tests_t;

void run_all_tests(const tests_t *tests, int count);

extern int test_passed;
extern int test_failed;

static int test_pass(char *msg) {
    printf("[PASS] %s \r\n", msg);
    test_passed++;
}

static int test_fail(char *msg) {
    printf("[FAIL] %s \r\n", msg);
    test_failed++;
}