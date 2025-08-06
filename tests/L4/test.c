#include "test.h"

#include <stdint.h>
#include <libstefi/util.h>

void run_all_tests(const tests_t *tests, int count) {
    for(int i = 0; i < count; i++) {
        printf("[%2d/%2d] Test %s \r\n", i, count, tests[i].test_name);
        tests[i].test_fn();
    }
}
