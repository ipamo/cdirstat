#include <errno.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "console.h"
#include "commons.h"

void _print_error(_error_nature_t nature, errno_t errno_, const char* file, int line, bool raw, const char* fmt, bool has_args, ...) {
    const char* errstr = NULL;
    char general_errstr[64];
    
    if (errno_ != 0) {        

        if (errno_ == E_WINDOWS_GET_LAST_ERROR) {
            errstr = "Windows error";
            //TODO: handle E_WINDOWS_GET_LAST_ERROR specifically
            //See: https://stackoverflow.com/questions/1387064/how-to-get-the-error-message-from-the-error-code-returned-by-getlasterror
        }
        else if (errno_ > E_WINDOWS_GET_LAST_ERROR) {
            _ERRNO_CUSTOM_MESSAGE;
        }

        if (! errstr) {
    #ifdef _WIN32
            if (strerror_s(general_errstr, sizeof(general_errstr), errno_) == 0)
                errstr = general_errstr;
    #else
            errstr = strerror_r(errno_, general_errstr, sizeof(general_errstr));
    #endif
        }
    }
    
    const char* nature_str;
    if (nature == _EN_ASSERTION) {
        nature_str = "ASSERTION FAILED";
    }
    else if (nature == _EN_FAILURE) {
        nature_str = "FAIL";
    }
    else {
        nature_str = "ERROR";
    }

    char* object;
    if (! fmt) {
        object = NULL;
    }
    else if (! has_args) {
        object = (char*) fmt;
    }
    else {
        va_list args;
        va_start (args, has_args);
        object = vsnprintf_a(NULL, fmt, args);
        va_end (args);
    }

#ifdef _DEBUG
    if (errno_ != 0) {
        (raw ? fprintf : fprintf_c)(stderr, "%s [%s:%d]%s%s%s%s (%d)\n", nature_str, file, line,
            object || errstr ? ": " : "",
            object ? object : "",
            object && errstr ? ": " : "",
            errstr ? errstr : "",
            errno_);
    }
    else {
        (raw ? fprintf : fprintf_c)(stderr, "%s [%s:%d]%s%s\n", nature_str, file, line,
            object ? ": " : "",
            object ? object : "");
    }
#else
    (void)file;
    (void)line;

    if (errno_ != 0) {
        (raw ? fprintf : fprintf_c)(stderr, "%s%s%s%s%s (%d)\n", nature_str,
            object || errstr ? ": " : "",
            object ? object : "",
            object && errstr ? ": " : "",
            errstr ? errstr : "",
            errno_);
    }
    else {
        (raw ? fprintf : fprintf_c)(stderr, "%s%s%s\n", nature_str,
            object ? ": " : "",
            object ? object : "");
    }
#endif

    if (fmt && has_args) {
        free(object);
    }
}

char* snprintf_a(int* len, const char *fmt, ...) {
    va_list args;
    va_start (args, fmt);
    char* buf = vsnprintf_a(len, fmt, args);
    va_end (args);
    return buf;
}

int snprintf_ra(char** buf, int* buf_size, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int len = vsnprintf_ra(buf, buf_size, fmt, args);
    va_end(args);
    return len;
}

int vsnprintf_ra(char** buf, int* buf_size, const char* fmt, va_list args) {
    if (!buf) {
        errno = E_INOUT_BUFFER_ARGUMENT_CANNOT_BE_NULL;
        return -1;
    }

    // Determine required length
    va_list args_copy;
    va_copy(args_copy, args);
    int len = vsnprintf(NULL, 0, fmt, args_copy);
    va_end(args_copy);

    if (len < 0) {
        errno = EOVERFLOW;
        return -1;
    }

    // Allocate or reallocate buffer if required
    int actual_size = len + 1;
    if (!*buf || !buf_size || *buf_size == 0) {
        *buf = (char*) malloc(actual_size * sizeof(char));
        if (buf_size) {
            *buf_size = actual_size;
        }
        if (! *buf) {
            errno = ENOMEM;
            return -1;
        }
    }
    else if (actual_size > *buf_size) {
        *buf = (char*) realloc(*buf, actual_size * sizeof(char));        
        *buf_size = actual_size;
        if (! *buf) {
            errno = ENOMEM;
            return -1;
        }
    }
    
    // Write formatted string to buffer
    return vsnprintf(*buf, actual_size, fmt, args);
}

char* vsnprintf_a(int* len, const char *fmt, va_list args) {
    char* buf = NULL;
    int size = 0;
    vsnprintf_ra(&buf, len ? &size : NULL, fmt, args);
    if (len) {
        *len = size - 1;
    }
    return buf;
}
