# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


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
CMAKE_COMMAND = /usr/local/src/clion-2018.2.2/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /usr/local/src/clion-2018.2.2/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /data/tmp/beasttest

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /data/tmp/beasttest/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/beasttest.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/beasttest.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/beasttest.dir/flags.make

CMakeFiles/beasttest.dir/main.cpp.o: CMakeFiles/beasttest.dir/flags.make
CMakeFiles/beasttest.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/data/tmp/beasttest/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/beasttest.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/beasttest.dir/main.cpp.o -c /data/tmp/beasttest/main.cpp

CMakeFiles/beasttest.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/beasttest.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /data/tmp/beasttest/main.cpp > CMakeFiles/beasttest.dir/main.cpp.i

CMakeFiles/beasttest.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/beasttest.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /data/tmp/beasttest/main.cpp -o CMakeFiles/beasttest.dir/main.cpp.s

# Object files for target beasttest
beasttest_OBJECTS = \
"CMakeFiles/beasttest.dir/main.cpp.o"

# External object files for target beasttest
beasttest_EXTERNAL_OBJECTS =

beasttest: CMakeFiles/beasttest.dir/main.cpp.o
beasttest: CMakeFiles/beasttest.dir/build.make
beasttest: CMakeFiles/beasttest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/data/tmp/beasttest/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable beasttest"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/beasttest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/beasttest.dir/build: beasttest

.PHONY : CMakeFiles/beasttest.dir/build

CMakeFiles/beasttest.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/beasttest.dir/cmake_clean.cmake
.PHONY : CMakeFiles/beasttest.dir/clean

CMakeFiles/beasttest.dir/depend:
	cd /data/tmp/beasttest/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /data/tmp/beasttest /data/tmp/beasttest /data/tmp/beasttest/cmake-build-debug /data/tmp/beasttest/cmake-build-debug /data/tmp/beasttest/cmake-build-debug/CMakeFiles/beasttest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/beasttest.dir/depend

