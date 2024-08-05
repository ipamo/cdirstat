/**
 * @file console.c
 * @brief Common implementations of `console.h`.
 * 
 * See `unix/_console.c` and `windows/_console.c` for platform-specific implementations.
 */
#include <stdarg.h>
#include "console.h"

int fprintf_c(FILE* const output, const char* const fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int written = vfprintf_c(output, fmt, args);
    va_end(args);
    return written;
}

int printf_c(const char *const fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int written = vprintf_c(fmt, args);
    va_end(args);
    return written;
}
