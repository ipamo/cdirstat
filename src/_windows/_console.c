/**
 * @file _console.c
 * @brief Windows-specific implementations of `console.h`.
 */
#include <stdbool.h>
#include <stdio.h>
#include <windows.h>
#include <io.h> // _isatty
#include "wstring.h"
#include "../commons.h"
#include "../console.h"

size_t fwrite_c(char* buf, size_t buf_size, FILE* file) {
    // Determine Windows console handle
    HANDLE h;
    if (file == stdout) {
        h = GetStdHandle(STD_OUTPUT_HANDLE);
    }
    else if (file == stderr) {
        h = GetStdHandle(STD_ERROR_HANDLE);
    }
    else {
        return fwrite(buf, 1, buf_size, file);
    }

    // Check if piped
    bool is_piped = _isatty(_fileno(file)) == 0;
    if (is_piped) {
        //
        // We cannot use `WriteConsoleW` otherwise pipe redirection will not be used and nothing will be displayed.
        // We output directly in UTF-8.
        // Accentuated chars will be broken except if Powershell is configured with UTF-8 encoding, using:
        //
        //      [Console]::OutputEncoding = [System.Text.Encoding]::UTF8
        //
        // N.B. This cannot be performed within our C program (with `SetConsoleCP(CP_UTF8)`) because this has to
        // be done BEFORE the piped process starts.
        //
        return fwrite(buf, buf_size, 1, file);
    }
    
    // Check Windows console handle
    if (h == NULL) {
        FAILE_R(E_WINDOWS_CONSOLE_HANDLE_NOT_FOUND);
    }
    if (h == INVALID_HANDLE_VALUE) {
        FAILE_R(E_INVALID_WINDOWS_CONSOLE_HANDLE);
    }

    // Convert chars
    int wcbuf_len;
    wchar_t* wcbuf = to_wide_a(&wcbuf_len, buf, (int)buf_size);

    // Export wide chars buffer to the console
    bool success = WriteConsoleW(h, wcbuf, (DWORD)wcbuf_len, NULL, NULL);
    
    // Cleanup
    free(wcbuf);

    return success ? buf_size : 0;
}

int vfprintf_c(FILE* const output, const char* const fmt, va_list args) {
    if (output == stdout || output == stderr) {
        char* buf;
        int buf_size;
        if (! (buf = vsnprintf_a(&buf_size, fmt, args))) {
            return -1;
        }
        size_t r;
        if ((r = fwrite_c(buf, buf_size, output)) < (size_t)buf_size) {
            return -1;
        }
        free(buf);
        return buf_size - 1;
    }
    else {
        return vfprintf_c(output, fmt, args);
    }
}

int vprintf_c(const char *const fmt, va_list args) {
    return vfprintf_c(stdout, fmt, args);
}
