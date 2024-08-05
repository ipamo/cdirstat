/**
 * @file _console.c
 * @brief Unix-specific implementations of `console.h`.
 */
#include <stdbool.h>
#include <stdio.h>
#include <cdirstat.h>
#include "../commons.h"
#include "../console.h"

size_t fwrite_c(char* buf, size_t buf_size, FILE* file) {
    return fwrite(buf, 1, buf_size, file);
}

int vfprintf_c(FILE* const output, const char* const fmt, va_list args) {
    return vfprintf(output, fmt, args);
}

int vprintf_c(const char *const fmt, va_list args) {
    return vprintf(fmt, args);
}
