cmake_minimum_required(VERSION 3.5.0)


# -----------------------------------------------------------------------------
# Parameters
#

if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if(NOT DEFINED CMAKE_SYSTEM_PROCESSOR)
    if (DEFINED CMAKE_HOST_SYSTEM_PROCESSOR)
        set(CMAKE_SYSTEM_PROCESSOR ${CMAKE_HOST_SYSTEM_PROCESSOR})
    else()
        set(CMAKE_SYSTEM_PROCESSOR x86_64)
    endif()
endif()

if(NOT DEFINED CMAKE_SYSTEM_NAME)
    set(CMAKE_SYSTEM_NAME ${CMAKE_HOST_SYSTEM_NAME})
endif()

if("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL Windows)
    set(CMAKE_C_COMPILER cl.exe)    
    enable_testing()
else()
    if("${CMAKE_SYSTEM_NAME}" STREQUAL Windows)
        # Cross compilation to Windows using gcc-mingw-w64
        set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
    else()
        # Standard Linux compilation
        set(CMAKE_C_COMPILER gcc)    
        enable_testing()
    endif()
endif()


# -----------------------------------------------------------------------------
# Project definition
#

# TODO: detect from git
set(CDIRSTAT_MAJOR_VERSION 0)
set(CDIRSTAT_MINOR_VERSION 1)
set(CDIRSTAT_PATCH_VERSION 0)
set(CDIRSTAT_TWEAK_VERSION dev)
add_definitions(-D_CDIRSTAT_MAJOR_VERSION="${CDIRSTAT_MAJOR_VERSION}")
add_definitions(-D_CDIRSTAT_MINOR_VERSION="${CDIRSTAT_MINOR_VERSION}")
add_definitions(-D_CDIRSTAT_PATCH_VERSION="${CDIRSTAT_PATCH_VERSION}")
if(CDIRSTAT_TWEAK_VERSION)
    add_definitions(-D_CDIRSTAT_TWEAK_VERSION="${CDIRSTAT_TWEAK_VERSION}")
endif()
add_definitions(-D_CDIRSTAT_COMMIT="hash")

project(cdirstat
    VERSION ${CDIRSTAT_MAJOR_VERSION}.${CDIRSTAT_MINOR_VERSION}.${CDIRSTAT_PATCH_VERSION}
    LANGUAGES C)

include(CTest)


# -----------------------------------------------------------------------------
# Compilation options
#
set(CMAKE_C_STANDARD 99)

if("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL Windows)
    # Use MSVC compiler
    set(CMAKE_C_FLAGS "/W4 /WX")
    set(CMAKE_C_FLAGS_DEBUG "/Zi -D_DEBUG")
    set(CMAKE_C_FLAGS_RELEASE "/O2")

    # Allow to remove absolute root directory by using __RELATIVE_FILE__ macro
    # See: https://stackoverflow.com/a/40947954
    string(LENGTH "${CMAKE_SOURCE_DIR}/" ABSOLUTE_ROOT_LEN)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_ABSOLUTE_ROOT_LEN=${ABSOLUTE_ROOT_LEN}")
else()
    # Use GCC compiler
    set(CMAKE_C_FLAGS "-Wall -Wextra -Wpedantic -Werror")
    set(CMAKE_C_FLAGS_DEBUG "-g -D_DEBUG")
    set(CMAKE_C_FLAGS_RELEASE "-O3")

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_GNU_SOURCE") # for strerror_r, etc.

    # Remove absolute root directory from __FILE__ macro
    # See: https://stackoverflow.com/a/53848526
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fmacro-prefix-map=${CMAKE_SOURCE_DIR}/= -ffile-prefix-map=${CMAKE_SOURCE_DIR}/=")
endif()


# -----------------------------------------------------------------------------
# Targets
#

# A function to strip compiled binaries
# See: https://blog.insane.engineer/post/cmake_strip_binaries/
function(utils_strip TARGET)
    add_custom_command(
        TARGET "${TARGET}" POST_BUILD
        DEPENDS "${TARGET}"
        COMMAND $<$<CONFIG:release>:${CMAKE_STRIP}>
        ARGS --strip-all $<TARGET_FILE:${TARGET}>
    )
endfunction()

file(GLOB src "src/*.c")
file(GLOB test-src "test/*.c")
if(${CMAKE_SYSTEM_NAME} STREQUAL Windows)
    file(GLOB platform-src "src/_windows/*.c")
    file(GLOB platform-test-src "test/_windows/*.c")
else()
    file(GLOB platform-src "src/_unix/*.c")
    file(GLOB platform-test-src "test/_unix/*.c")
endif()

include_directories(include)

add_library(lib SHARED ${src} ${platform-src})
set_target_properties(lib PROPERTIES OUTPUT_NAME cdirstat)
utils_strip(lib)

add_executable(cli src/cli/main.c)
set_target_properties(cli PROPERTIES OUTPUT_NAME cdirstat)
target_link_libraries(cli PRIVATE lib)
if(${CMAKE_C_COMPILER} MATCHES x86_64-w64-mingw32-gcc)
    target_link_options(cli PRIVATE -municode) # Use 'wmain' function instead of 'main'
endif()
utils_strip(cli)

foreach (test-file ${test-src} ${platform-test-src})
    get_filename_component(test_name ${test-file} NAME_WLE)

    add_executable(${test_name} ${test-file})
    target_link_libraries(${test_name} PRIVATE lib cmocka)
    
    add_test(NAME ${test_name} COMMAND $<TARGET_FILE:${test_name}>)
endforeach ()
