#include <cdirstat.h>

#ifdef _WIN32
int wmain(int argc, wchar_t* argv[]) {
    return cdirstat_cli(argc, argv);
}
#else
int main(int argc, char* argv[]) {
    return cdirstat_cli(argc, argv);
}
#endif
