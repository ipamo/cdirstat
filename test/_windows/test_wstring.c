#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>
#include <stdlib.h>
#include "../../src/_windows/wstring.h"

void test_to_wide(void** state) {
    (void)state;
    
    #define WC_SIZE 3
    wchar_t buf[WC_SIZE*sizeof(wchar_t)];
    int len;

    len = to_wide(buf, WC_SIZE, "Hé", -1);
    assert_int_equal(2, len);
    assert_true(len >= 0 && len < WC_SIZE);
    assert_memory_equal("H\0\xe9\0\0", buf, 2*len + 1);

    len = to_wide(buf, WC_SIZE, "Héhé", -1);
    assert_int_equal(4, len);
    assert_false(len >= 0 && len < WC_SIZE); // Buffer is not long enough
    assert_memory_equal("H\0\xe9\0\0", buf, 5); // Previous is kept because buffer is not long enough

    len = to_wide(buf, WC_SIZE, "Ô", -1);
    assert_int_equal(1, len);
    assert_true(len >= 0 && len < WC_SIZE);
    assert_memory_equal("\xd4\0\0", buf, 2*len + 1);
}

void test_to_wide_a(void** state) {
    (void)state;
    wchar_t* buf = NULL;
    int len;

    buf = to_wide_a(&len, "Hé", -1);
    assert_int_equal(2, len);
    assert_memory_equal("H\0\xe9\0\0", buf, 2*len + 1);
    free(buf);

    buf = to_wide_a(&len, "Héhé", -1);
    assert_int_equal(4, len);
    assert_memory_equal("H\0\xe9\0h\0\xe9\0\0", buf, 2*len + 1);
    free(buf);

    buf = to_wide_a(&len, "Ô", -1);
    assert_int_equal(1, len);
    assert_memory_equal("\xd4\0\0", buf, 2*len + 1);
    free(buf);
}

void test_to_wide_ra(void** state) {
    (void)state;
    wchar_t* buf = NULL;
    int buf_size;
    int len;

    assert_int_equal(2, (len = to_wide_ra(&buf, &buf_size, "Hé", -1)));
    assert_int_equal(3, buf_size);
    assert_memory_equal("H\0\xe9\0\0", buf, 2*len + 1);

    assert_int_equal(4, (len = to_wide_ra(&buf, &buf_size, "Héhé", -1)));
    assert_int_equal(5, buf_size);
    assert_memory_equal("H\0\xe9\0h\0\xe9\0\0", buf, 2*len + 1);

    assert_int_equal(1, (len = to_wide_ra(&buf, &buf_size, "Ô", -1)));
    assert_int_equal(5, buf_size);
    assert_memory_equal("\xd4\0\0", buf, 2*len + 1);

    free(buf);
}

void test_to_utf8(void** state) {
    (void)state;
    
    #define SIZE 4
    char buf[SIZE];
    int len;

    len = to_utf8(buf, SIZE, L"H", -1);
    assert_int_equal(1, len);
    assert_true(len >= 0 && len < SIZE);
    assert_string_equal("H", buf);

    len = to_utf8(buf, SIZE, (wchar_t*)"H\0\xe9\0\0", -1);
    assert_int_equal(3, len); // é => 2 chars
    assert_true(len >= 0 && len < SIZE);
    assert_string_equal("Hé", buf);

    len = to_utf8(buf, SIZE, (wchar_t*)"H\0\xe9\0h\0\xe9\0\0", -1);
    assert_int_equal(6, len);
    assert_false(len >= 0 && len < SIZE); // Buffer is not long enough
    assert_string_equal("Hé", buf); // Previous is kept because buffer is not long enough

    len = to_utf8(buf, SIZE, (wchar_t*)"\xd4\0\0", -1);
    assert_int_equal(2, len);
    assert_true(len >= 0 && len < SIZE);
    assert_string_equal("Ô", buf);
}

void test_to_utf8_a(void** state) {
    (void)state;
    char* buf = NULL;
    int len;

    buf = to_utf8_a(&len, (wchar_t*)"H\0\xe9\0\0", -1);
    assert_int_equal(3, len);
    assert_string_equal("Hé", buf);
    free(buf);

    buf = to_utf8_a(&len, (wchar_t*)"H\0\xe9\0h\0\xe9\0\0", -1);
    assert_int_equal(6, len);
    assert_string_equal("Héhé", buf);
    free(buf);

    buf = to_utf8_a(&len, (wchar_t*)"\xd4\0\0", -1);
    assert_int_equal(2, len);
    assert_string_equal("Ô", buf);
    free(buf);
}

void test_to_utf8_ra(void** state) {
    (void)state;
    char* buf = NULL;
    int buf_size;
    int len;

    assert_int_equal(3, (len = to_utf8_ra(&buf, &buf_size, (wchar_t*)"H\0\xe9\0\0", -1)));
    assert_int_equal(4, buf_size);
    assert_string_equal("Hé", buf);

    assert_int_equal(6, (len = to_utf8_ra(&buf, &buf_size, (wchar_t*)"H\0\xe9\0h\0\xe9\0\0", -1)));
    assert_int_equal(7, buf_size);
    assert_string_equal("Héhé", buf);

    assert_int_equal(2, (len = to_utf8_ra(&buf, &buf_size, (wchar_t*)"\xd4\0\0", -1)));
    assert_int_equal(7, buf_size);
    assert_string_equal("Ô", buf);

    free(buf);
}

int main() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_to_wide),
        cmocka_unit_test(test_to_wide_a),
        cmocka_unit_test(test_to_wide_ra),
        cmocka_unit_test(test_to_utf8),
        cmocka_unit_test(test_to_utf8_a),
        cmocka_unit_test(test_to_utf8_ra)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
