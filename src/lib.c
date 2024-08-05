#include <string.h>
#include <cdirstat.h>
#include "argparse.h"
#include "console.h"

#ifdef _CDIRSTAT_TWEAK_VERSION // provided by cmake
    #define _CDIRSTAT_VERSION _CDIRSTAT_MAJOR_VERSION "." _CDIRSTAT_MINOR_VERSION "." _CDIRSTAT_PATCH_VERSION "-" _CDIRSTAT_TWEAK_VERSION // provided by cmake
#else
    #define _CDIRSTAT_VERSION _CDIRSTAT_MAJOR_VERSION "." _CDIRSTAT_MINOR_VERSION "." _CDIRSTAT_PATCH_VERSION // provided by cmake
#endif


const char* cdirstat_version() {
    return _CDIRSTAT_VERSION;
}

const char* cdirstat_commit() {
    return _CDIRSTAT_COMMIT; // provided by cmake
}

int _cdirstat_cli(argparse_t* ap) {
    argparse_next(ap); // ignore name of program

    // TODO!
    printf_c("%d: >%s<\n", strlen("Hé"), "Hé");
    printf_c("%d: >%s<\n", strlen("He"), "He");

    printf_c("argc=%d\n", ap->argc);
    char* arg;
    while ((arg = argparse_next(ap))) {
        printf_c("- %s [%d]\n", arg, strlen(arg));
    }
    return 0;
}

#ifdef _WIN32
int cdirstat_cli(int argc, wchar_t* argv[]) {
    argparse_t* ap = argparse_wcreate(argc, argv);
#else
int cdirstat_cli(int argc, char* argv[]) {    
    argparse_t* ap = argparse_create(argc, argv);
#endif
    int r = _cdirstat_cli(ap);
    argparse_free(ap);
    return r;
}
