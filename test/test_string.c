#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>
#include <stdlib.h>
#include "../src/commons.h"

void test_a(void** state) {
    (void)state;
    char* buf = NULL;
    int buf_size;
    
    buf = snprintf_a(&buf_size, "Hi: %d", 123);
    assert_int_equal(7, buf_size);
    assert_non_null(buf);

    free(buf);
}

void test_ra(void** state) {
    (void)state;
    char* buf = NULL;
    int buf_size;

    assert_int_equal(7, snprintf_ra(&buf, &buf_size, "Hi: %d", 123));
    assert_int_equal(8, buf_size);
    assert_string_equal("Hi: 123", buf);

    assert_int_equal(10, snprintf_ra(&buf, &buf_size, "Hi: %d", 123456));
    assert_int_equal(11, buf_size);
    assert_string_equal("Hi: 123456", buf);

    assert_int_equal(3, snprintf_ra(&buf, &buf_size, "Hi!"));
    assert_int_equal(11, buf_size);
    assert_string_equal("Hi!", buf);

    free(buf);
}

int main() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_a),
        cmocka_unit_test(test_ra),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
