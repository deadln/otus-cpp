# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ocelot/programming/otus/otus-cpp/lesson1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ocelot/programming/otus/otus-cpp/lesson1/build

# Include any dependencies generated for this target.
include CMakeFiles/test_version.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/test_version.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test_version.dir/flags.make

CMakeFiles/test_version.dir/test_version.cpp.o: CMakeFiles/test_version.dir/flags.make
CMakeFiles/test_version.dir/test_version.cpp.o: ../test_version.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ocelot/programming/otus/otus-cpp/lesson1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/test_version.dir/test_version.cpp.o"
	/usr/lib/ccache/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_version.dir/test_version.cpp.o -c /home/ocelot/programming/otus/otus-cpp/lesson1/test_version.cpp

CMakeFiles/test_version.dir/test_version.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_version.dir/test_version.cpp.i"
	/usr/lib/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ocelot/programming/otus/otus-cpp/lesson1/test_version.cpp > CMakeFiles/test_version.dir/test_version.cpp.i

CMakeFiles/test_version.dir/test_version.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_version.dir/test_version.cpp.s"
	/usr/lib/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ocelot/programming/otus/otus-cpp/lesson1/test_version.cpp -o CMakeFiles/test_version.dir/test_version.cpp.s

# Object files for target test_version
test_version_OBJECTS = \
"CMakeFiles/test_version.dir/test_version.cpp.o"

# External object files for target test_version
test_version_EXTERNAL_OBJECTS =

test_version: CMakeFiles/test_version.dir/test_version.cpp.o
test_version: CMakeFiles/test_version.dir/build.make
test_version: /usr/lib/x86_64-linux-gnu/libboost_unit_test_framework.so.1.71.0
test_version: libhelloworld.a
test_version: CMakeFiles/test_version.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ocelot/programming/otus/otus-cpp/lesson1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable test_version"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_version.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test_version.dir/build: test_version

.PHONY : CMakeFiles/test_version.dir/build

CMakeFiles/test_version.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test_version.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test_version.dir/clean

CMakeFiles/test_version.dir/depend:
	cd /home/ocelot/programming/otus/otus-cpp/lesson1/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ocelot/programming/otus/otus-cpp/lesson1 /home/ocelot/programming/otus/otus-cpp/lesson1 /home/ocelot/programming/otus/otus-cpp/lesson1/build /home/ocelot/programming/otus/otus-cpp/lesson1/build /home/ocelot/programming/otus/otus-cpp/lesson1/build/CMakeFiles/test_version.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test_version.dir/depend

