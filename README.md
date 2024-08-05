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

    sudo apt install cmake build-essential gdb mingw-w64 gdb-mingw-w64 libcmocka-dev file

### Pre-requisites for Windows hosts

[Download](https://aka.ms/vs/17/release/vs_buildtools.exe) and install Visual Studio Build Tools. Enable the following components:
- TODO

### Build instructions

Prepare:

    mkdir -p build
    cd build
    cmake ..

Build:

    cmake --build .

Run unit tests:

    ctest test

To run the command-line interface, run `./cdirstat` on Linux or `.\cdirstat.exe` on Windows (from the `build` directory).


## Legal

This project is licensed under the terms of the [MIT license](https://raw.githubusercontent.com/ipamo/cdirstat/main/LICENSE.txt).
