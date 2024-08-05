#include <windows.h>
#include "../commons.h"
#include "wstring.h"

int to_wide(wchar_t* wc, int wc_size, char* mb, int mb_len) {
    if (mb_len == -1) {
        mb_len = (int) strlen(mb);
    }

    // Determine required number of wide chars
    int wc_len = MultiByteToWideChar(CP_UTF8, 0, mb, mb_len, NULL, 0);
    if (wc_len == 0) {
        errno = E_WINDOWS_GET_LAST_ERROR;
        return -1;
    }

    // Check buffer size
    if (wc_len >= wc_size) {
        return wc_len;
    }

    // Write to wide chars buffer
    if (MultiByteToWideChar(CP_UTF8, 0, mb, mb_len, wc, wc_len) == 0) {
        errno = E_WINDOWS_GET_LAST_ERROR;
        return -1;
    }
    wc[wc_len] = '\0';
    return wc_len;
}

int to_utf8(char* mb, int mb_size, wchar_t* wc, int wc_len) {
    if (wc_len == -1) {
        wc_len = (int) wcslen(wc);
    }

    // Determine required number of multi-byte chars
    int mb_len = WideCharToMultiByte(CP_UTF8, 0, wc, wc_len, NULL, 0, NULL, NULL);
    if (mb_len == 0) {
        errno = E_WINDOWS_GET_LAST_ERROR;
        return -1;
    }

    // Check buffer size
    if (mb_len >= mb_size) {
        return mb_len;
    }

    // Write to multi-byte chars buffer
    if (WideCharToMultiByte(CP_UTF8, 0, wc, wc_len, mb, mb_len, NULL, NULL) == 0) {
        errno = E_WINDOWS_GET_LAST_ERROR;
        return -1;
    }
    mb[mb_len] = '\0';
    return mb_len;  
}

wchar_t* to_wide_a(int* wc_len, char* mb, int mb_len) {
    wchar_t* wc = NULL;
    int wc_size = 0;
    to_wide_ra(&wc, wc_len ? &wc_size : NULL, mb, mb_len);
    if (wc_len) {
        *wc_len = wc_size - 1;
    }
    return wc;
}

char* to_utf8_a(int* mb_len, wchar_t* wc, int wc_len) {
    char* mb = NULL;
    int mb_size = 0;
    to_utf8_ra(&mb, mb_len ? &mb_size : NULL, wc, wc_len);
    if (mb_len) {
        *mb_len = mb_size - 1;
    }
    return mb;
}

int to_wide_ra(wchar_t** wc, int* wc_size, char* mb, int mb_len) {
    if (!wc) {
        errno = E_INOUT_BUFFER_ARGUMENT_CANNOT_BE_NULL;
        return -1;
    }
    if (mb_len == -1) {
        mb_len = (int) strlen(mb);
    }

    // Determine required number of wide chars
    int wc_len_ = MultiByteToWideChar(CP_UTF8, 0, mb, mb_len, NULL, 0);
    if (wc_len_ == 0) {
        errno = E_WINDOWS_GET_LAST_ERROR;
        return -1;
    }

    // Allocate wide chars buffer
    int actual_wc_size = wc_len_ + 1;
    if (!*wc || !wc_size || *wc_size == 0) {
        *wc = (wchar_t*) malloc(actual_wc_size * sizeof(wchar_t));
        if (wc_size) {
            *wc_size = actual_wc_size;
        }
        if (! *wc) {
            errno = ENOMEM;
            return -1;
        }
    }
    else if (actual_wc_size > *wc_size) {
        *wc = (wchar_t*) realloc(*wc, actual_wc_size * sizeof(wchar_t)); 
        *wc_size = actual_wc_size;
        if (! *wc) {
            errno = ENOMEM;
            return -1;
        }
    }

    // Write to wide chars buffer
    if (MultiByteToWideChar(CP_UTF8, 0, mb, mb_len, *wc, wc_len_) == 0) {
        errno = ENOMEM;
        return -1;
    }
    *(*wc + wc_len_) = '\0';
    return wc_len_;
}

int to_utf8_ra(char** mb, int* mb_size, wchar_t* wc, int wc_len) {
    if (!mb) {
        errno = E_INOUT_BUFFER_ARGUMENT_CANNOT_BE_NULL;
        return -1;
    }

    if (wc_len == -1) {
        wc_len = (int) wcslen(wc);
    }

    // Determine required number of multi-byte chars
    int mb_len_ = WideCharToMultiByte(CP_UTF8, 0, wc, wc_len, NULL, 0, NULL, NULL);
    if (mb_len_ == 0) {
        errno = E_WINDOWS_GET_LAST_ERROR;
        return -1;
    }

    // Allocate multi-byte chars buffer
    int actual_mb_size = mb_len_ + 1;
    if (!*mb || !mb_size || *mb_size == 0) {
        *mb = (char*) malloc(actual_mb_size * sizeof(char));
        if (mb_size) {
            *mb_size = actual_mb_size;
        }
        if (! *mb) {
            errno = ENOMEM;
            return -1;
        }
    }
    else if (actual_mb_size > *mb_size) {
        *mb = (char*) realloc(*mb, actual_mb_size * sizeof(char)); 
        *mb_size = actual_mb_size;
        if (! *mb) {
            errno = ENOMEM;
            return -1;
        }
    }

    // Write to multi-byte chars buffer
    if (WideCharToMultiByte(CP_UTF8, 0, wc, wc_len, *mb, mb_len_, NULL, NULL) == 0) {
        errno = E_WINDOWS_GET_LAST_ERROR;
        return -1;
    }
    (*mb)[mb_len_] = '\0';
    return mb_len_;  
}
