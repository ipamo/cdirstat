/**
 * @file argparse.c
 * @brief Common implementations of `console.h`.
 * 
 * See `windows/_argparse.c` for platform-specific implementations.
 */
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "commons.h"
#include "console.h"
#include "argparse.h"

const char* ARGPARSE_FAIL = "_FAIL";

argparse_t* argparse_create(int argc, char* argv[]) {
    argparse_t* ap = (argparse_t*) malloc(sizeof(argparse_t));
    ASSERT_ALLOC(ap);

    ap->argc = argc;
    
#ifdef _WIN32
    // On Windows we can reuse the existing argv because it has already been allocated by our process
    // (when tranformed from `wchar_t* argv[]` in `argparse_wcreate`).
    ap->argv = argv;
#else
    ap->argv = (char**) malloc(argc * sizeof(char*));
    ASSERT_ALLOC(ap->argv);
    for (int i = 0; i < argc; i++) {
        ap->argv[i] = argv[i];
    }
#endif

    ap->allocated = calloc(argc, sizeof(bool));
    ap->next_index = 0;

    return ap;
}

void argparse_free(argparse_t* ap) {
    for (size_t i = 0; i < ap->argc; i++) {
        if (ap->allocated[i]) {
            free(ap->argv[i]);
        }
    }
    
#ifdef _WIN32
    // ap->argv not allocated in `argparse_create`
#else
    free(ap->argv);
#endif

    free(ap);
}

char* argparse_next(argparse_t* ap) {
    if (ap->next_index >= ap->argc) {
        return NULL;
    }
    
    char* arg = ap->argv[ap->next_index++];

    size_t len = (size_t) strlen(arg);
    if (len <= 2) {
        return arg;
    }
    
    if (arg[0] != '-' || strchr(arg+1, '-')) {
        return arg;
    }

    // This is a list of short options
    // We insert them here, shifting the remaining arrays
    size_t shift_count = len - 2;
    
    ap->argv = (char**) realloc(ap->argv, (ap->argc + shift_count) * sizeof(char*));
    ASSERT_ALLOC(ap->argv);

    // Move higher-index arguments (if any) to newly created spaces
    for (size_t cur_i = ap->argc - 1; cur_i >= ap->next_index; cur_i--) {
        size_t new_i = cur_i + shift_count;

        ap->argv[new_i] = ap->argv[cur_i];
    }

    // Create new single-char arguments
    for (size_t i = 0; i < shift_count; i++) {
        size_t new_i = ap->next_index + i;

        char* opt_arg = (char*) malloc(3 * sizeof(char));
        opt_arg[0] = '-';
        opt_arg[1] = arg[2 + i];
        opt_arg[2] = '\0';

        ap->argv[new_i] = opt_arg;
        ap->allocated[new_i] = true;
    }

    ap->argc += shift_count;

    // Keep only the first option as current arg
    arg[2] = '\0';
    return arg;
}

char* argparse_opt_value(argparse_t* ap, char* default_value) {
    bool missing = false;
    if (ap->next_index >= ap->argc) { // option is last
        missing = true;
    }
    else if (ap->argv[ap->next_index][0] == '-') { // option is followed by another option
        missing = true;
    }

    if (missing) {
        if (default_value == ARGPARSE_FAIL) {
            fprintf_c(stderr, "FAIL: Missing value for option %s", ap->argv[ap->next_index-1]);
            exit(EINVAL);
        }
        return default_value;
    }
    
    return ap->argv[ap->next_index++];
}
