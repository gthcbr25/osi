cmake_minimum_required(VERSION 3.29)
project(untitled4 C)

set(CMAKE_C_STANDARD 11)

add_executable(untitled4 main.c
        src/parent.c
        src/child.c)
