# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

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

#Suppress display of executed commands.
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
CMAKE_SOURCE_DIR = /Users/liammurphy/Documents/Raymarcher

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/liammurphy/Documents/Raymarcher/cmake-build-release

# Utility rule file for ContinuousSubmit.

# Include the progress variables for this target.
include unit-tests/CMakeFiles/ContinuousSubmit.dir/progress.make

unit-tests/CMakeFiles/ContinuousSubmit:
	cd /Users/liammurphy/Documents/Raymarcher/cmake-build-release/unit-tests && /Applications/CLion.app/Contents/bin/cmake/mac/bin/ctest -D ContinuousSubmit

ContinuousSubmit: unit-tests/CMakeFiles/ContinuousSubmit
ContinuousSubmit: unit-tests/CMakeFiles/ContinuousSubmit.dir/build.make

.PHONY : ContinuousSubmit

# Rule to build all files generated by this target.
unit-tests/CMakeFiles/ContinuousSubmit.dir/build: ContinuousSubmit

.PHONY : unit-tests/CMakeFiles/ContinuousSubmit.dir/build

unit-tests/CMakeFiles/ContinuousSubmit.dir/clean:
	cd /Users/liammurphy/Documents/Raymarcher/cmake-build-release/unit-tests && $(CMAKE_COMMAND) -P CMakeFiles/ContinuousSubmit.dir/cmake_clean.cmake
.PHONY : unit-tests/CMakeFiles/ContinuousSubmit.dir/clean

unit-tests/CMakeFiles/ContinuousSubmit.dir/depend:
	cd /Users/liammurphy/Documents/Raymarcher/cmake-build-release && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/liammurphy/Documents/Raymarcher /Users/liammurphy/Documents/Raymarcher/unit-tests /Users/liammurphy/Documents/Raymarcher/cmake-build-release /Users/liammurphy/Documents/Raymarcher/cmake-build-release/unit-tests /Users/liammurphy/Documents/Raymarcher/cmake-build-release/unit-tests/CMakeFiles/ContinuousSubmit.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : unit-tests/CMakeFiles/ContinuousSubmit.dir/depend

