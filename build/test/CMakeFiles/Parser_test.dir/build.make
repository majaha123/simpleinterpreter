# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.22

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = C:\Users\user123\Desktop\11class\cmake-3.22.0-rc2-windows-x86_64\bin\cmake.exe

# The command to remove a file.
RM = C:\Users\user123\Desktop\11class\cmake-3.22.0-rc2-windows-x86_64\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\user123\Desktop\11class\ownInterpreter

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\user123\Desktop\11class\ownInterpreter\build

# Include any dependencies generated for this target.
include test/CMakeFiles/Parser_test.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include test/CMakeFiles/Parser_test.dir/compiler_depend.make

# Include the progress variables for this target.
include test/CMakeFiles/Parser_test.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/Parser_test.dir/flags.make

test/CMakeFiles/Parser_test.dir/Parser_test.cpp.obj: test/CMakeFiles/Parser_test.dir/flags.make
test/CMakeFiles/Parser_test.dir/Parser_test.cpp.obj: test/CMakeFiles/Parser_test.dir/includes_CXX.rsp
test/CMakeFiles/Parser_test.dir/Parser_test.cpp.obj: ../test/Parser_test.cpp
test/CMakeFiles/Parser_test.dir/Parser_test.cpp.obj: test/CMakeFiles/Parser_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\user123\Desktop\11class\ownInterpreter\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/Parser_test.dir/Parser_test.cpp.obj"
	cd /d C:\Users\user123\Desktop\11class\ownInterpreter\build\test && C:\mingw32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT test/CMakeFiles/Parser_test.dir/Parser_test.cpp.obj -MF CMakeFiles\Parser_test.dir\Parser_test.cpp.obj.d -o CMakeFiles\Parser_test.dir\Parser_test.cpp.obj -c C:\Users\user123\Desktop\11class\ownInterpreter\test\Parser_test.cpp

test/CMakeFiles/Parser_test.dir/Parser_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Parser_test.dir/Parser_test.cpp.i"
	cd /d C:\Users\user123\Desktop\11class\ownInterpreter\build\test && C:\mingw32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\user123\Desktop\11class\ownInterpreter\test\Parser_test.cpp > CMakeFiles\Parser_test.dir\Parser_test.cpp.i

test/CMakeFiles/Parser_test.dir/Parser_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Parser_test.dir/Parser_test.cpp.s"
	cd /d C:\Users\user123\Desktop\11class\ownInterpreter\build\test && C:\mingw32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\user123\Desktop\11class\ownInterpreter\test\Parser_test.cpp -o CMakeFiles\Parser_test.dir\Parser_test.cpp.s

# Object files for target Parser_test
Parser_test_OBJECTS = \
"CMakeFiles/Parser_test.dir/Parser_test.cpp.obj"

# External object files for target Parser_test
Parser_test_EXTERNAL_OBJECTS =

test/Parser_test.exe: test/CMakeFiles/Parser_test.dir/Parser_test.cpp.obj
test/Parser_test.exe: test/CMakeFiles/Parser_test.dir/build.make
test/Parser_test.exe: lib/libgtest_main.a
test/Parser_test.exe: libInterpreter.a
test/Parser_test.exe: lib/libgtest.a
test/Parser_test.exe: test/CMakeFiles/Parser_test.dir/linklibs.rsp
test/Parser_test.exe: test/CMakeFiles/Parser_test.dir/objects1.rsp
test/Parser_test.exe: test/CMakeFiles/Parser_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\user123\Desktop\11class\ownInterpreter\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Parser_test.exe"
	cd /d C:\Users\user123\Desktop\11class\ownInterpreter\build\test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\Parser_test.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/Parser_test.dir/build: test/Parser_test.exe
.PHONY : test/CMakeFiles/Parser_test.dir/build

test/CMakeFiles/Parser_test.dir/clean:
	cd /d C:\Users\user123\Desktop\11class\ownInterpreter\build\test && $(CMAKE_COMMAND) -P CMakeFiles\Parser_test.dir\cmake_clean.cmake
.PHONY : test/CMakeFiles/Parser_test.dir/clean

test/CMakeFiles/Parser_test.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\user123\Desktop\11class\ownInterpreter C:\Users\user123\Desktop\11class\ownInterpreter\test C:\Users\user123\Desktop\11class\ownInterpreter\build C:\Users\user123\Desktop\11class\ownInterpreter\build\test C:\Users\user123\Desktop\11class\ownInterpreter\build\test\CMakeFiles\Parser_test.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/Parser_test.dir/depend

