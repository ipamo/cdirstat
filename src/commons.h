#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "errors.h"


// ----------------------------------------------------------------------------
// Error handling
//

#ifndef _WIN32
#define errno_t int
#endif

// Used by FAIL* and ASSERT* macros
typedef enum {
    _EN_ERROR,
    _EN_FAILURE,
    _EN_ASSERTION,
} _error_nature_t;

// Used by FAIL* and ASSERT* macros
void _print_error(_error_nature_t nature, errno_t errno_, const char* file, int line, bool raw, const char* fmt, bool no_args, ...);

#ifdef _DEBUG

#ifdef _ABSOLUTE_ROOT_LEN
#define __RELATIVE_FILE__ ((__FILE__) + (_ABSOLUTE_ROOT_LEN))
#else
#define __RELATIVE_FILE__ __FILE__
#endif

#define FAIL(errno_, single) { _print_error(_EN_FAILURE, errno_, __RELATIVE_FILE__, __LINE__, false, single, false); exit(1); }
#define FAIL0() { _print_error(_EN_FAILURE, 0, __RELATIVE_FILE__, __LINE__, false, NULL, false); exit(1); }
#define FAILE(errno_) { _print_error(_EN_FAILURE, errno_, __RELATIVE_FILE__, __LINE__, false, NULL, false); exit(1); }
#define FAILF(errno_, fmt, ...) { _print_error(_EN_FAILURE, errno_, __RELATIVE_FILE__, __LINE__, false, fmt, true, ##__VA_ARGS__); exit(1); }
#define ASSERT(expr) if (!(expr)) { _print_error(_EN_ASSERTION, 0, __RELATIVE_FILE__, __LINE__, false, #expr, false); exit(1); }
#define ASSERT_ALLOC(pointer) if (!(pointer)) { _print_error(_EN_ASSERTION, ENOMEM, __RELATIVE_FILE__, __LINE__, false, #pointer, false); exit(1); }

#define FAIL_R(errno_, single) { _print_error(_EN_FAILURE, errno_, __RELATIVE_FILE__, __LINE__, true, single, false); exit(1); }
#define FAIL0_R() { _print_error(_EN_FAILURE, 0, __RELATIVE_FILE__, __LINE__, true, NULL, false); exit(1); }
#define FAILE_R(errno_) { _print_error(_EN_FAILURE, errno_, __RELATIVE_FILE__, __LINE__, true, NULL, false); exit(1); }
#define FAILF_R(errno_, fmt, ...) { _print_error(_EN_FAILURE, errno_, __RELATIVE_FILE__, __LINE__, true, fmt, true, ##__VA_ARGS__); exit(1); }
#define ASSERT_R(expr) if (!(expr)) { _print_error(_EN_ASSERTION, 0, __RELATIVE_FILE__, __LINE__, false, #expr, false); exit(1); }
#define ASSERT_ALLOC_R(pointer) if (!(pointer)) { _print_error(_EN_ASSERTION, ENOMEM, __RELATIVE_FILE__, __LINE__, true, #pointer, false); exit(1); }

#else

#define FAIL(errno_, single) { _print_error(_EN_FAILURE, errno_, NULL, 0, false, single, false); exit(1); }
#define FAIL0() { _print_error(_EN_FAILURE, 0, NULL, 0, false, NULL, false); exit(1); }
#define FAILE(errno_) { _print_error(_EN_FAILURE, errno_, NULL, 0, false, NULL, false); exit(1); }
#define FAILF(errno_, fmt, ...) { _print_error(_EN_FAILURE, errno_, NULL, 0, false, fmt, true, ##__VA_ARGS__); exit(1); }
#define ASSERT(expr) if (!(expr)) { _print_error(_EN_ASSERTION, 0, NULL, 0, false, #expr, false); exit(1); }
#define ASSERT_ALLOC(pointer) if (!(pointer)) { _print_error(_EN_ASSERTION, ENOMEM, NULL, 0, false, NULL, false); exit(1); }

#define FAIL_R(errno_, single) { _print_error(_EN_FAILURE, errno_, NULL, 0, true, single, false); exit(1); }
#define FAIL0_R() { _print_error(_EN_FAILURE, 0, NULL, 0, true, NULL, false); exit(1); }
#define FAILE_R(errno_) { _print_error(_EN_FAILURE, errno_, NULL, 0, true, NULL, false); exit(1); }
#define FAILF_R(errno_, fmt, ...) { _print_error(_EN_FAILURE, errno_, NULL, 0, true, fmt, true, ##__VA_ARGS__); exit(1); }
#define ASSERT_R(expr) if (!(expr)) { _print_error(_EN_ASSERTION, 0, NULL, 0, true, #expr, false); exit(1); }
#define ASSERT_ALLOC_R(pointer) if (!(pointer)) { _print_error(_EN_ASSERTION, ENOMEM, NULL, 0, true, NULL, false); exit(1); }

#endif


// ----------------------------------------------------------------------------
// `vsnprintf` extensions
//

/**
 * Allocating version of `snprintf`.
 * 
 * @param len [out, optional] Number of characters (excluding terminating null character) of the allocated string. Use NULL if not necessary.
 * @param fmt [in] Format string.
 * 
 * @param ... [in] Arguments to the format string.
 * 
 * @return Allocated string, or NULL if error (`errno` is set).
 */
char* snprintf_a(int* len, const char *fmt, ...);

/**
 * Re-allocating version of `snprintf`.
 * 
 * @param buf [in/out] Pointer to a buffer to allocate or reallocate.
 * @param buf_size [in/out] Pointer to the size of the buffer (INCLUDING terminating null character).
 * @param fmt [in] Format string.
 * @param ... [in] Arguments to the format string.
 * 
 * @return Number of characters (excluding terminating null character) that would have been written if the buffer had been sufficiently large,
 * or -1 in case of error (then `errno` is set).
 * 
 * @note The parameters and return values are similar to `snprintf`.
 */
int snprintf_ra(char** buf, int* buf_size, const char* fmt, ...);

/**
 * Allocating version of `vsnprintf`.
 * 
 * @param len [out, optional] Number of characters (excluding terminating null character) of the allocated string. Use NULL if not necessary.
 * @param fmt [in] Format string.
 * @param args [in] Arguments to the format string.
 * 
 * @return Allocated string, or NULL in case of error (then `errno` is set).
 */
char* vsnprintf_a(int* len, const char *fmt, va_list args);

/**
 * Re-allocating version of `vsnprintf`.
 * 
 * @param buf [in/out] Pointer to a buffer to allocate or reallocate.
 * @param buf_size [in/out] Pointer to the size of the buffer (INCLUDING terminating null character).
 * @param fmt [in] Format string.
 * @param args [in] Arguments to the format string.
 * 
 * @return Number of characters (excluding terminating null character) that would have been written if the buffer had been sufficiently large,
 * or -1 in case of error (then `errno` is set).
 * 
 * @note The parameters and return values are similar to `snprintf`.
 */
int vsnprintf_ra(char** buf, int* buf_size, const char* fmt, va_list args);
