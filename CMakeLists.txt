cmake_minimum_required(VERSION 3.17)

#set(CMAKE_CXX_STANDARD 99)
set(CMAKE_C_STANDARD 99)
set(CMAKE_OSX_DEPLOYMENT_TARGET 11.0)

# Set <PROJNAME> to be your project name. We set C here as the target language.
project(Chess VERSION 0.1.0 LANGUAGES C)

# If you want to see "under the hood" of CMake, uncomment this:
# set(CMAKE_VERBOSE_MAKEFILE ON)

# Define variables containing compiler flags for gcc
set(MTM_FLAGS_DEBUG "-std=c99 --pedantic-errors -Wall -Werror -DNDEBUG")
set(MTM_FLAGS_RELEASE "${MTM_FLAGS_DEBUG}")

set(CMAKE_C_FLAGS ${MTM_FLAGS_DEBUG})


include_directories(.)

add_executable(Chess
        map.c
        player_map.c
        game_map.c
        tournament_map.c
        chessSystem.c
        chessSystemTestsExample.c
        )
