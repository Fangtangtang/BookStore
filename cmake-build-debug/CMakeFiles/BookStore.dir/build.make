# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/c/repos/BookStore

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/repos/BookStore/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/BookStore.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/BookStore.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/BookStore.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/BookStore.dir/flags.make

CMakeFiles/BookStore.dir/main.cpp.o: CMakeFiles/BookStore.dir/flags.make
CMakeFiles/BookStore.dir/main.cpp.o: ../main.cpp
CMakeFiles/BookStore.dir/main.cpp.o: CMakeFiles/BookStore.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/repos/BookStore/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/BookStore.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/BookStore.dir/main.cpp.o -MF CMakeFiles/BookStore.dir/main.cpp.o.d -o CMakeFiles/BookStore.dir/main.cpp.o -c /mnt/c/repos/BookStore/main.cpp

CMakeFiles/BookStore.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/BookStore.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/repos/BookStore/main.cpp > CMakeFiles/BookStore.dir/main.cpp.i

CMakeFiles/BookStore.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/BookStore.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/repos/BookStore/main.cpp -o CMakeFiles/BookStore.dir/main.cpp.s

CMakeFiles/BookStore.dir/linkList.cpp.o: CMakeFiles/BookStore.dir/flags.make
CMakeFiles/BookStore.dir/linkList.cpp.o: ../linkList.cpp
CMakeFiles/BookStore.dir/linkList.cpp.o: CMakeFiles/BookStore.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/repos/BookStore/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/BookStore.dir/linkList.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/BookStore.dir/linkList.cpp.o -MF CMakeFiles/BookStore.dir/linkList.cpp.o.d -o CMakeFiles/BookStore.dir/linkList.cpp.o -c /mnt/c/repos/BookStore/linkList.cpp

CMakeFiles/BookStore.dir/linkList.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/BookStore.dir/linkList.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/repos/BookStore/linkList.cpp > CMakeFiles/BookStore.dir/linkList.cpp.i

CMakeFiles/BookStore.dir/linkList.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/BookStore.dir/linkList.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/repos/BookStore/linkList.cpp -o CMakeFiles/BookStore.dir/linkList.cpp.s

CMakeFiles/BookStore.dir/account.cpp.o: CMakeFiles/BookStore.dir/flags.make
CMakeFiles/BookStore.dir/account.cpp.o: ../account.cpp
CMakeFiles/BookStore.dir/account.cpp.o: CMakeFiles/BookStore.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/repos/BookStore/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/BookStore.dir/account.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/BookStore.dir/account.cpp.o -MF CMakeFiles/BookStore.dir/account.cpp.o.d -o CMakeFiles/BookStore.dir/account.cpp.o -c /mnt/c/repos/BookStore/account.cpp

CMakeFiles/BookStore.dir/account.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/BookStore.dir/account.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/repos/BookStore/account.cpp > CMakeFiles/BookStore.dir/account.cpp.i

CMakeFiles/BookStore.dir/account.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/BookStore.dir/account.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/repos/BookStore/account.cpp -o CMakeFiles/BookStore.dir/account.cpp.s

# Object files for target BookStore
BookStore_OBJECTS = \
"CMakeFiles/BookStore.dir/main.cpp.o" \
"CMakeFiles/BookStore.dir/linkList.cpp.o" \
"CMakeFiles/BookStore.dir/account.cpp.o"

# External object files for target BookStore
BookStore_EXTERNAL_OBJECTS =

BookStore: CMakeFiles/BookStore.dir/main.cpp.o
BookStore: CMakeFiles/BookStore.dir/linkList.cpp.o
BookStore: CMakeFiles/BookStore.dir/account.cpp.o
BookStore: CMakeFiles/BookStore.dir/build.make
BookStore: CMakeFiles/BookStore.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/repos/BookStore/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable BookStore"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/BookStore.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/BookStore.dir/build: BookStore
.PHONY : CMakeFiles/BookStore.dir/build

CMakeFiles/BookStore.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/BookStore.dir/cmake_clean.cmake
.PHONY : CMakeFiles/BookStore.dir/clean

CMakeFiles/BookStore.dir/depend:
	cd /mnt/c/repos/BookStore/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/repos/BookStore /mnt/c/repos/BookStore /mnt/c/repos/BookStore/cmake-build-debug /mnt/c/repos/BookStore/cmake-build-debug /mnt/c/repos/BookStore/cmake-build-debug/CMakeFiles/BookStore.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/BookStore.dir/depend

