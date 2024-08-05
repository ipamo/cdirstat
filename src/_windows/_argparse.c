/**
 * @file _argparse.c
 * @brief Windows-specific implementations of `argparse.h`.
 */
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "wstring.h"
#include "../commons.h"
#include "../argparse.h"

argparse_t* argparse_wcreate(int argc, wchar_t* argv[]) {
	char** mb_argv  = (char**) malloc(argc * sizeof(char*));
	if (! mb_argv) {
        ASSERT_ALLOC(mb_argv);
	}

	for (int i = 0; i < argc; i++) {
        mb_argv[i] = to_utf8_a(NULL, argv[i], -1);
		if (! mb_argv[i]) {
			FAILE(errno);
		}
	}
    
    argparse_t* argparse = argparse_create(argc, mb_argv);
	for (int i = 0; i < argc; i++) {
        argparse->allocated[i] = true;
	}
    return argparse;
}
