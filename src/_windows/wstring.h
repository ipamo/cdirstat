/**
 * @file wstring.h
 * @brief Conversions between multi-byte UTF-8 strings and wide-char UTF-16 strings.
 * 
 * Multi-byte UTF-8 strings are assumed to be used on Unix systems, and is used internally by our program.
 * Wide-char UTF-16 strings are used by Windows-specific functions.
 */
#include <errno.h>
#include <wchar.h>

/** 
 * Convert a multi-byte (UTF-8) string to a wide-char string.
 * 
 * @param wc Wide-char buffer that will be written.
 * @param wc_size Size of the buffer to write (INCLUDING terminating null character), expressed as a number of wide characters.
 * @param mb Input UTF-8 string.
 * @param mb_len Number of chars of the input UTF-8 string, or `-1` to calculate.
 * 
 * @return Number of wide characters (excluding terminating null character) that would have been written if the buffer had been sufficiently large,
 * or -1 in case of error (then `errno` is set).
 */
int to_wide(wchar_t* wc, int wc_size, char* mb, int mb_len);

int to_utf8(char* mb, int mb_size, wchar_t* wc, int wc_len);

/** 
 * Allocating version of `to_wide`.
 * 
 * @param wc_len [out, optional] Number of wide characters (excluding terminating null character) of the allocated string. Use NULL if not necessary.
 * @param mb Input UTF-8 string.
 * @param mb_len Number of characters (excluding terminating zero) of the input UTF-8 string, or `-1` to calculate.
 * 
 * @return Newly allocated wide-char string.
 */
wchar_t* to_wide_a(int* wc_len, char* mb, int mb_len);

char* to_utf8_a(int* mb_len, wchar_t* wc, int wc_len);

/** 
 * Re-allocating version of `to_wide`.
 * 
 * @param wc [in/out] Pointer to a wide-char buffer to allocate or reallocate.
 * @param wc_size [in/out] Pointer to the size of the buffer (INCLUDING terminating null character), expressed as a number of wide characters.
 * @param mb [in] Input UTF-8 string.
 * @param mb_len [in] Number of characters (excluding terminating zero) of the input UTF-8 string, or `-1` to calculate.
 * 
 * @return Number of wide characters written (excluding terminating zero),
 * or -1 in case of error (then `errno` is set).
 * 
 * @note The parameters and return values are similar to `snprintf`.
 */
int to_wide_ra(wchar_t** wc, int* wc_size, char* mb, int mb_len);

int to_utf8_ra(char** mb, int* mb_size, wchar_t* wc, int wc_len);
