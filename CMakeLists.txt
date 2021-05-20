cmake_minimum_required(VERSION 3.17)
project(Chess)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Chess
        chessSystem.h
        chessSystem.c
        main.c
        libmap.a)
