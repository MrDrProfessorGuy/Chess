# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/guycohen/OneDrive - Technion/1 Courses/מבוא לתכנות מערכות/HW/HW1/Chess"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/guycohen/OneDrive - Technion/1 Courses/מבוא לתכנות מערכות/HW/HW1/Chess/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/Chess.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Chess.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Chess.dir/flags.make

CMakeFiles/Chess.dir/map.c.o: CMakeFiles/Chess.dir/flags.make
CMakeFiles/Chess.dir/map.c.o: ../map.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/guycohen/OneDrive - Technion/1 Courses/מבוא לתכנות מערכות/HW/HW1/Chess/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Chess.dir/map.c.o"
	/usr/local/bin/gcc-10 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Chess.dir/map.c.o   -c "/Users/guycohen/OneDrive - Technion/1 Courses/מבוא לתכנות מערכות/HW/HW1/Chess/map.c"

CMakeFiles/Chess.dir/map.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Chess.dir/map.c.i"
	/usr/local/bin/gcc-10 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/Users/guycohen/OneDrive - Technion/1 Courses/מבוא לתכנות מערכות/HW/HW1/Chess/map.c" > CMakeFiles/Chess.dir/map.c.i

CMakeFiles/Chess.dir/map.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Chess.dir/map.c.s"
	/usr/local/bin/gcc-10 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/Users/guycohen/OneDrive - Technion/1 Courses/מבוא לתכנות מערכות/HW/HW1/Chess/map.c" -o CMakeFiles/Chess.dir/map.c.s

CMakeFiles/Chess.dir/player_map.c.o: CMakeFiles/Chess.dir/flags.make
CMakeFiles/Chess.dir/player_map.c.o: ../player_map.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/guycohen/OneDrive - Technion/1 Courses/מבוא לתכנות מערכות/HW/HW1/Chess/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/Chess.dir/player_map.c.o"
	/usr/local/bin/gcc-10 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Chess.dir/player_map.c.o   -c "/Users/guycohen/OneDrive - Technion/1 Courses/מבוא לתכנות מערכות/HW/HW1/Chess/player_map.c"

CMakeFiles/Chess.dir/player_map.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Chess.dir/player_map.c.i"
	/usr/local/bin/gcc-10 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/Users/guycohen/OneDrive - Technion/1 Courses/מבוא לתכנות מערכות/HW/HW1/Chess/player_map.c" > CMakeFiles/Chess.dir/player_map.c.i

CMakeFiles/Chess.dir/player_map.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Chess.dir/player_map.c.s"
	/usr/local/bin/gcc-10 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/Users/guycohen/OneDrive - Technion/1 Courses/מבוא לתכנות מערכות/HW/HW1/Chess/player_map.c" -o CMakeFiles/Chess.dir/player_map.c.s

CMakeFiles/Chess.dir/game_map.c.o: CMakeFiles/Chess.dir/flags.make
CMakeFiles/Chess.dir/game_map.c.o: ../game_map.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/guycohen/OneDrive - Technion/1 Courses/מבוא לתכנות מערכות/HW/HW1/Chess/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/Chess.dir/game_map.c.o"
	/usr/local/bin/gcc-10 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Chess.dir/game_map.c.o   -c "/Users/guycohen/OneDrive - Technion/1 Courses/מבוא לתכנות מערכות/HW/HW1/Chess/game_map.c"

CMakeFiles/Chess.dir/game_map.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Chess.dir/game_map.c.i"
	/usr/local/bin/gcc-10 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/Users/guycohen/OneDrive - Technion/1 Courses/מבוא לתכנות מערכות/HW/HW1/Chess/game_map.c" > CMakeFiles/Chess.dir/game_map.c.i

CMakeFiles/Chess.dir/game_map.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Chess.dir/game_map.c.s"
	/usr/local/bin/gcc-10 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/Users/guycohen/OneDrive - Technion/1 Courses/מבוא לתכנות מערכות/HW/HW1/Chess/game_map.c" -o CMakeFiles/Chess.dir/game_map.c.s

CMakeFiles/Chess.dir/tournament_map.c.o: CMakeFiles/Chess.dir/flags.make
CMakeFiles/Chess.dir/tournament_map.c.o: ../tournament_map.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/guycohen/OneDrive - Technion/1 Courses/מבוא לתכנות מערכות/HW/HW1/Chess/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/Chess.dir/tournament_map.c.o"
	/usr/local/bin/gcc-10 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Chess.dir/tournament_map.c.o   -c "/Users/guycohen/OneDrive - Technion/1 Courses/מבוא לתכנות מערכות/HW/HW1/Chess/tournament_map.c"

CMakeFiles/Chess.dir/tournament_map.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Chess.dir/tournament_map.c.i"
	/usr/local/bin/gcc-10 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/Users/guycohen/OneDrive - Technion/1 Courses/מבוא לתכנות מערכות/HW/HW1/Chess/tournament_map.c" > CMakeFiles/Chess.dir/tournament_map.c.i

CMakeFiles/Chess.dir/tournament_map.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Chess.dir/tournament_map.c.s"
	/usr/local/bin/gcc-10 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/Users/guycohen/OneDrive - Technion/1 Courses/מבוא לתכנות מערכות/HW/HW1/Chess/tournament_map.c" -o CMakeFiles/Chess.dir/tournament_map.c.s

CMakeFiles/Chess.dir/chessSystem.c.o: CMakeFiles/Chess.dir/flags.make
CMakeFiles/Chess.dir/chessSystem.c.o: ../chessSystem.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/guycohen/OneDrive - Technion/1 Courses/מבוא לתכנות מערכות/HW/HW1/Chess/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/Chess.dir/chessSystem.c.o"
	/usr/local/bin/gcc-10 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Chess.dir/chessSystem.c.o   -c "/Users/guycohen/OneDrive - Technion/1 Courses/מבוא לתכנות מערכות/HW/HW1/Chess/chessSystem.c"

CMakeFiles/Chess.dir/chessSystem.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Chess.dir/chessSystem.c.i"
	/usr/local/bin/gcc-10 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/Users/guycohen/OneDrive - Technion/1 Courses/מבוא לתכנות מערכות/HW/HW1/Chess/chessSystem.c" > CMakeFiles/Chess.dir/chessSystem.c.i

CMakeFiles/Chess.dir/chessSystem.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Chess.dir/chessSystem.c.s"
	/usr/local/bin/gcc-10 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/Users/guycohen/OneDrive - Technion/1 Courses/מבוא לתכנות מערכות/HW/HW1/Chess/chessSystem.c" -o CMakeFiles/Chess.dir/chessSystem.c.s

CMakeFiles/Chess.dir/chessSystemTestsExample.c.o: CMakeFiles/Chess.dir/flags.make
CMakeFiles/Chess.dir/chessSystemTestsExample.c.o: ../chessSystemTestsExample.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/guycohen/OneDrive - Technion/1 Courses/מבוא לתכנות מערכות/HW/HW1/Chess/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/Chess.dir/chessSystemTestsExample.c.o"
	/usr/local/bin/gcc-10 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Chess.dir/chessSystemTestsExample.c.o   -c "/Users/guycohen/OneDrive - Technion/1 Courses/מבוא לתכנות מערכות/HW/HW1/Chess/chessSystemTestsExample.c"

CMakeFiles/Chess.dir/chessSystemTestsExample.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Chess.dir/chessSystemTestsExample.c.i"
	/usr/local/bin/gcc-10 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/Users/guycohen/OneDrive - Technion/1 Courses/מבוא לתכנות מערכות/HW/HW1/Chess/chessSystemTestsExample.c" > CMakeFiles/Chess.dir/chessSystemTestsExample.c.i

CMakeFiles/Chess.dir/chessSystemTestsExample.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Chess.dir/chessSystemTestsExample.c.s"
	/usr/local/bin/gcc-10 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/Users/guycohen/OneDrive - Technion/1 Courses/מבוא לתכנות מערכות/HW/HW1/Chess/chessSystemTestsExample.c" -o CMakeFiles/Chess.dir/chessSystemTestsExample.c.s

# Object files for target Chess
Chess_OBJECTS = \
"CMakeFiles/Chess.dir/map.c.o" \
"CMakeFiles/Chess.dir/player_map.c.o" \
"CMakeFiles/Chess.dir/game_map.c.o" \
"CMakeFiles/Chess.dir/tournament_map.c.o" \
"CMakeFiles/Chess.dir/chessSystem.c.o" \
"CMakeFiles/Chess.dir/chessSystemTestsExample.c.o"

# External object files for target Chess
Chess_EXTERNAL_OBJECTS =

Chess: CMakeFiles/Chess.dir/map.c.o
Chess: CMakeFiles/Chess.dir/player_map.c.o
Chess: CMakeFiles/Chess.dir/game_map.c.o
Chess: CMakeFiles/Chess.dir/tournament_map.c.o
Chess: CMakeFiles/Chess.dir/chessSystem.c.o
Chess: CMakeFiles/Chess.dir/chessSystemTestsExample.c.o
Chess: CMakeFiles/Chess.dir/build.make
Chess: CMakeFiles/Chess.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/Users/guycohen/OneDrive - Technion/1 Courses/מבוא לתכנות מערכות/HW/HW1/Chess/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_7) "Linking C executable Chess"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Chess.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Chess.dir/build: Chess

.PHONY : CMakeFiles/Chess.dir/build

CMakeFiles/Chess.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Chess.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Chess.dir/clean

CMakeFiles/Chess.dir/depend:
	cd "/Users/guycohen/OneDrive - Technion/1 Courses/מבוא לתכנות מערכות/HW/HW1/Chess/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/guycohen/OneDrive - Technion/1 Courses/מבוא לתכנות מערכות/HW/HW1/Chess" "/Users/guycohen/OneDrive - Technion/1 Courses/מבוא לתכנות מערכות/HW/HW1/Chess" "/Users/guycohen/OneDrive - Technion/1 Courses/מבוא לתכנות מערכות/HW/HW1/Chess/cmake-build-debug" "/Users/guycohen/OneDrive - Technion/1 Courses/מבוא לתכנות מערכות/HW/HW1/Chess/cmake-build-debug" "/Users/guycohen/OneDrive - Technion/1 Courses/מבוא לתכנות מערכות/HW/HW1/Chess/cmake-build-debug/CMakeFiles/Chess.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/Chess.dir/depend

