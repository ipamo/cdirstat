/**
 * @file argparse.h
 * @brief Argument parsing utility.
 */
#include <stdbool.h>
#include <stddef.h>

#ifdef _WIN32
#include <wchar.h>
#endif


extern const char* ARGPARSE_FAIL;

typedef struct {
    size_t argc;
    char** argv; // [argc]
    bool* allocated; // [argc]
    size_t next_index;
} argparse_t;

#ifdef _WIN32
/**
 * Create an argument parser from Windows `wmain()` function.
 * This function receive arguments as wide-character (UTF-16) strings. See :c:func::eded.
 */
argparse_t* argparse_wcreate(int argc, wchar_t* argv[]);
#endif

/**
 * Create an argument parser from standard `main()` function.
 * This function receive arguments as UTF-8 strings.
 */
argparse_t* argparse_create(int argc, char* argv[]);

/**
 * Cleanup argparse.
 */
void argparse_free(argparse_t* ap);

/**
 * Get the next argument value as an UTF-8 string.
 */
char* argparse_next(argparse_t* ap);

/**
 * Get the value of the previous arg, considered as an option.
 * 
 * @param ap Parser handle.
 * @param default_value Value to use if the option has no value (i.e. the option was the last argument, or the option is followed by another option).
 *      You may use `NULL` (returns `NULL`) or `ARGPARSE_FAIL` (exit with `EINVAL`: _Invalid argument_).
 * 
 * @return Value of the option.
 */
char* argparse_opt_value(argparse_t* ap, char* default_value);
