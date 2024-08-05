#define E_WINDOWS_GET_LAST_ERROR 2000000000

#define E_MISSING_REQUIRED_ARGUMENT 2000000001
#define E_MISSING_REQUIRED_ARGUMENT_M "Missing required argument"

#define E_INSUFFICIENT_BUFFER 2000000002
#define E_INSUFFICIENT_BUFFER_M "Insufficient buffer"

#define E_INOUT_BUFFER_ARGUMENT_CANNOT_BE_NULL 2000000003
#define E_INOUT_BUFFER_ARGUMENT_CANNOT_BE_NULL_M "In/out buffer argument cannot be null"

#define E_WINDOWS_CONSOLE_HANDLE_NOT_FOUND 2000000004
#define E_WINDOWS_CONSOLE_HANDLE_NOT_FOUND_M "Windows console handle not found"

#define E_INVALID_WINDOWS_CONSOLE_HANDLE 2000000005
#define E_INVALID_WINDOWS_CONSOLE_HANDLE_M "Invalid Windows console handle"

#ifdef _DEBUG
#define _ERRNO_CUSTOM_MESSAGE { \
    if (errno_ == E_MISSING_REQUIRED_ARGUMENT) \
        errstr = "Missing required argument"; \
    if (errno_ == E_INOUT_BUFFER_ARGUMENT_CANNOT_BE_NULL) \
        errstr = "In/out buffer argument cannot be null"; \
    if (errno_ == E_WINDOWS_CONSOLE_HANDLE_NOT_FOUND) \
        errstr = "Windows console handle not found"; \
    if (errno_ == E_INVALID_WINDOWS_CONSOLE_HANDLE) \
        errstr = "Invalid Windows console handle"; \
}
#else
#define _ERRNO_CUSTOM_MESSAGE { \
    if (errno_ == E_MISSING_REQUIRED_ARGUMENT) \
        errstr = "Missing required argument"; \
}
#endif
