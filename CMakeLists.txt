cmake_minimum_required(VERSION 3.17)
project(Chess)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Chess
        chessSystem.c
        game_map.c
        player_map.c
        tournament_map.c
        libmap.a)
