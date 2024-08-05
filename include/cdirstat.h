/**
 * @file cdirstat.h
 * @brief API for library `cdirstat`.
 * @version 0.1.0
 */
#ifdef _WIN32
#include <stddef.h> // wchar_t
#endif


/** Get version for the compiled library. */
const char* cdirstat_version();

/** Get build Git commit hash for the compiled library. */
const char* cdirstat_commit();

#ifdef _WIN32
int cdirstat_cli(int argc, wchar_t* argv[]);
#else
int cdirstat_cli(int argc, char* argv[]);
#endif
