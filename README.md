cdirstat
========

**WORK IN PROGRESS**

A cross-platform command-line application and library to analyze a (possibly huge) directory recursively, and produce a CSV stat report.

Written in C, inspired from the following projects:

- [windirstat](https://windirstat.net/) : Windows only.
- [qdirstat](https://github.com/shundhammer/qdirstat) : Linux only.
- [dirstat](https://github.com/GaelGirodon/dirstat) : written in Go. Produce `.json` and `.html` reports containing statistics. Very nice `.html` reports. However the report files tend to be quite huge, and are generated only at the end of the program (meanwhile accumulated in the program memory, which grows as the json file grows).


## Documentation

TODO: generate using Doxygen.


## Build

### Pre-requisites for Linux hosts

Install:

    sudo apt install cmake build-essential gdb mingw-w64 gdb-mingw-w64 libcmocka-dev file doxygen

To build tests when cross-compiling to Windows, also compile and install `cmocka` for mingw-w64:

    sudo git clone https://git.cryptomilk.org/projects/cmocka.git /usr/local/src/

    cd /usr/local/src/cmocka/

    sudo cmake . -B build \
        -DCMAKE_INSTALL_PREFIX=/usr/x86_64-w64-mingw32 \
        -DCMAKE_SYSTEM_NAME=Windows \
        -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc \
        -DCMAKE_BUILD_TYPE=Release

    sudo cmake --build build

    sudo cmake --install build


### Pre-requisites for Windows hosts

[Download](https://aka.ms/vs/17/release/vs_buildtools.exe) and install Visual Studio Build Tools and enable CMake, MSVC, C runtime and Windows SDK components.

Add CMake's `bin` directory (example: `C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin`) in your `PATH` environment variable.

Specify the generator matching your version of VS Build Tools. Example:

    $env:CMAKE_GENERATOR="Visual Studio 17 2022"

NOTE: The generator may also be configured in your `.vscode/settings.json`. Example:

    {
        "cmake.generator": "Visual Studio 17 2022"
    }

Build cmocka - TODO: follow Linux instructions instead?:

    mkdir build
    cd build
    cmake -G "Visual Studio 17 2022" path/to/cmocka-1.1.7
    cmake --build . --config Release

Install cmocka (from an elevated prompt) :

    $MSVC_PATH="C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.40.33807"

    cp build\src\Release\cmocka.dll "$MSVC_PATH\bin\Hostx64\x64\"
    cp build\src\Release\cmocka.lib "$MSVC_PATH\lib\x64\"
    cp include\cmocka.h             "$MSVC_PATH\14.40.33807\include\"
    
### Build instructions (common to Windows and Linux)

Prepare:

    cmake . -B build

Build:

    cmake --build build

Run unit tests:

    cmake --build build --target run_tests

To run the command-line interface, run `./cdirstat` on Linux or `.\Debug\cdirstat.exe` on Windows (from the `build` directory).


## Legal

This project is licensed under the terms of the [MIT license](https://raw.githubusercontent.com/ipamo/cdirstat/main/LICENSE.txt).
