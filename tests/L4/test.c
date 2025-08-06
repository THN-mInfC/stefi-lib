#include "test.h"
#include <libstefi/util.h>

int test_passed = 0;
int test_failed = 0;

void run_all_tests(const tests_t *tests, int count) {
    for(int i = 0; i < count; i++) {
        printf("[%2d/%2d] Test: %s \r\n", i+1, count, tests[i].test_name);
        tests[i].test_fn();
    }

    printf("\r\n Summary: %d passed %d failed \r\n", test_passed, test_failed);
}