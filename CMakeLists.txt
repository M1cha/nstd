cmake_minimum_required(VERSION 3.0)
project(nstd C)

add_compile_options(
    -Wall
    -Wextra
    -Wno-unused-parameter
    -Wno-unused-variable
    -Wconversion
    -Wcast-qual
    -Werror
    -O2
    -std=gnu99
)

add_library(nstd
    buf.c
    conversion.c
    string.c
    time.c
)
target_include_directories(nstd PUBLIC
    include
)
