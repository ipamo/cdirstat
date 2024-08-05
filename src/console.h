/**
 * @file console.h
 * @brief Versions of `fwrite` and `printf`-likes compatible, with Windows console.
 */
#include <stdio.h>

/**
 * A version of `fwrite` for characters, compatible with Windows console (handles Windows encoding and pipe redirections correctly).
 * 
 * @param buf String buffer to write.
 * @param buf_size Size of the buffer to write (INCLUDING terminating null character).
 * @param file Target file.
 * 
 * @return Size written (INCLUDING terminating zero), or a value lower than `buf_size` in case of error (then `errno` is set).
 */
size_t fwrite_c(char* buf, size_t buf_size, FILE* file);

/**
 * A version of `vfprintf` compatible with Windows console (handles Windows encoding and pipe redirections correctly).
 * @return Number of chars (excluding terminating zero) written, or -1 in case of error (then `errno` is set).
 */
int vfprintf_c(FILE* const output, const char* const fmt, va_list args);

/**
 * A version of `fprintf` compatible with Windows console (handles Windows encoding and pipe redirections correctly).
 * @return Number of chars (excluding terminating zero) written, or -1 in case of error (then `errno` is set).
 */
int fprintf_c(FILE* const output, const char* const fmt, ...);

/**
 * A version of `vprintf` compatible with Windows console (handles Windows encoding and pipe redirections correctly).
 * @return Number of chars (excluding terminating zero) written, or -1 in case of error (then `errno` is set).
 */
int vprintf_c(const char *const fmt, va_list args);

/**
 * A version of `printf` compatible with Windows console (handles Windows encoding and pipe redirections correctly).
 * @return Number of chars (excluding terminating zero) written, or -1 in case of error (then `errno` is set).
 */
int printf_c(const char *const fmt, ...);
