# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.29

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
CMAKE_COMMAND = "C:/Program Files/CMake/bin/cmake.exe"

# The command to remove a file.
RM = "C:/Program Files/CMake/bin/cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:/GH_Local/M-trees

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:/GH_Local/M-trees/build

# Include any dependencies generated for this target.
include CMakeFiles/MTrees.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/MTrees.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/MTrees.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/MTrees.dir/flags.make

CMakeFiles/MTrees.dir/src/cluster_utils.cpp.obj: CMakeFiles/MTrees.dir/flags.make
CMakeFiles/MTrees.dir/src/cluster_utils.cpp.obj: CMakeFiles/MTrees.dir/includes_CXX.rsp
CMakeFiles/MTrees.dir/src/cluster_utils.cpp.obj: C:/GH_Local/M-trees/src/cluster_utils.cpp
CMakeFiles/MTrees.dir/src/cluster_utils.cpp.obj: CMakeFiles/MTrees.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:/GH_Local/M-trees/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/MTrees.dir/src/cluster_utils.cpp.obj"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/MTrees.dir/src/cluster_utils.cpp.obj -MF CMakeFiles/MTrees.dir/src/cluster_utils.cpp.obj.d -o CMakeFiles/MTrees.dir/src/cluster_utils.cpp.obj -c C:/GH_Local/M-trees/src/cluster_utils.cpp

CMakeFiles/MTrees.dir/src/cluster_utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/MTrees.dir/src/cluster_utils.cpp.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:/GH_Local/M-trees/src/cluster_utils.cpp > CMakeFiles/MTrees.dir/src/cluster_utils.cpp.i

CMakeFiles/MTrees.dir/src/cluster_utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/MTrees.dir/src/cluster_utils.cpp.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:/GH_Local/M-trees/src/cluster_utils.cpp -o CMakeFiles/MTrees.dir/src/cluster_utils.cpp.s

CMakeFiles/MTrees.dir/src/nearest_neighbour.cpp.obj: CMakeFiles/MTrees.dir/flags.make
CMakeFiles/MTrees.dir/src/nearest_neighbour.cpp.obj: CMakeFiles/MTrees.dir/includes_CXX.rsp
CMakeFiles/MTrees.dir/src/nearest_neighbour.cpp.obj: C:/GH_Local/M-trees/src/nearest_neighbour.cpp
CMakeFiles/MTrees.dir/src/nearest_neighbour.cpp.obj: CMakeFiles/MTrees.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:/GH_Local/M-trees/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/MTrees.dir/src/nearest_neighbour.cpp.obj"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/MTrees.dir/src/nearest_neighbour.cpp.obj -MF CMakeFiles/MTrees.dir/src/nearest_neighbour.cpp.obj.d -o CMakeFiles/MTrees.dir/src/nearest_neighbour.cpp.obj -c C:/GH_Local/M-trees/src/nearest_neighbour.cpp

CMakeFiles/MTrees.dir/src/nearest_neighbour.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/MTrees.dir/src/nearest_neighbour.cpp.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:/GH_Local/M-trees/src/nearest_neighbour.cpp > CMakeFiles/MTrees.dir/src/nearest_neighbour.cpp.i

CMakeFiles/MTrees.dir/src/nearest_neighbour.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/MTrees.dir/src/nearest_neighbour.cpp.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:/GH_Local/M-trees/src/nearest_neighbour.cpp -o CMakeFiles/MTrees.dir/src/nearest_neighbour.cpp.s

CMakeFiles/MTrees.dir/src/nearest_pair.cpp.obj: CMakeFiles/MTrees.dir/flags.make
CMakeFiles/MTrees.dir/src/nearest_pair.cpp.obj: CMakeFiles/MTrees.dir/includes_CXX.rsp
CMakeFiles/MTrees.dir/src/nearest_pair.cpp.obj: C:/GH_Local/M-trees/src/nearest_pair.cpp
CMakeFiles/MTrees.dir/src/nearest_pair.cpp.obj: CMakeFiles/MTrees.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:/GH_Local/M-trees/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/MTrees.dir/src/nearest_pair.cpp.obj"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/MTrees.dir/src/nearest_pair.cpp.obj -MF CMakeFiles/MTrees.dir/src/nearest_pair.cpp.obj.d -o CMakeFiles/MTrees.dir/src/nearest_pair.cpp.obj -c C:/GH_Local/M-trees/src/nearest_pair.cpp

CMakeFiles/MTrees.dir/src/nearest_pair.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/MTrees.dir/src/nearest_pair.cpp.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:/GH_Local/M-trees/src/nearest_pair.cpp > CMakeFiles/MTrees.dir/src/nearest_pair.cpp.i

CMakeFiles/MTrees.dir/src/nearest_pair.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/MTrees.dir/src/nearest_pair.cpp.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:/GH_Local/M-trees/src/nearest_pair.cpp -o CMakeFiles/MTrees.dir/src/nearest_pair.cpp.s

CMakeFiles/MTrees.dir/src/ss_method.cpp.obj: CMakeFiles/MTrees.dir/flags.make
CMakeFiles/MTrees.dir/src/ss_method.cpp.obj: CMakeFiles/MTrees.dir/includes_CXX.rsp
CMakeFiles/MTrees.dir/src/ss_method.cpp.obj: C:/GH_Local/M-trees/src/ss_method.cpp
CMakeFiles/MTrees.dir/src/ss_method.cpp.obj: CMakeFiles/MTrees.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:/GH_Local/M-trees/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/MTrees.dir/src/ss_method.cpp.obj"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/MTrees.dir/src/ss_method.cpp.obj -MF CMakeFiles/MTrees.dir/src/ss_method.cpp.obj.d -o CMakeFiles/MTrees.dir/src/ss_method.cpp.obj -c C:/GH_Local/M-trees/src/ss_method.cpp

CMakeFiles/MTrees.dir/src/ss_method.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/MTrees.dir/src/ss_method.cpp.i"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:/GH_Local/M-trees/src/ss_method.cpp > CMakeFiles/MTrees.dir/src/ss_method.cpp.i

CMakeFiles/MTrees.dir/src/ss_method.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/MTrees.dir/src/ss_method.cpp.s"
	g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:/GH_Local/M-trees/src/ss_method.cpp -o CMakeFiles/MTrees.dir/src/ss_method.cpp.s

# Object files for target MTrees
MTrees_OBJECTS = \
"CMakeFiles/MTrees.dir/src/cluster_utils.cpp.obj" \
"CMakeFiles/MTrees.dir/src/nearest_neighbour.cpp.obj" \
"CMakeFiles/MTrees.dir/src/nearest_pair.cpp.obj" \
"CMakeFiles/MTrees.dir/src/ss_method.cpp.obj"

# External object files for target MTrees
MTrees_EXTERNAL_OBJECTS =

tmp/MTrees.exe: CMakeFiles/MTrees.dir/src/cluster_utils.cpp.obj
tmp/MTrees.exe: CMakeFiles/MTrees.dir/src/nearest_neighbour.cpp.obj
tmp/MTrees.exe: CMakeFiles/MTrees.dir/src/nearest_pair.cpp.obj
tmp/MTrees.exe: CMakeFiles/MTrees.dir/src/ss_method.cpp.obj
tmp/MTrees.exe: CMakeFiles/MTrees.dir/build.make
tmp/MTrees.exe: CMakeFiles/MTrees.dir/linkLibs.rsp
tmp/MTrees.exe: CMakeFiles/MTrees.dir/objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=C:/GH_Local/M-trees/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable tmp/MTrees.exe"
	"C:/Program Files/CMake/bin/cmake.exe" -E rm -f CMakeFiles/MTrees.dir/objects.a
	C:/msys64/ucrt64/bin/ar.exe qc CMakeFiles/MTrees.dir/objects.a @CMakeFiles/MTrees.dir/objects1.rsp
	g++ -Wl,--verbose -Wl,--whole-archive CMakeFiles/MTrees.dir/objects.a -Wl,--no-whole-archive -o tmp/MTrees.exe -Wl,--out-implib,libMTrees.dll.a -Wl,--major-image-version,0,--minor-image-version,0 @CMakeFiles/MTrees.dir/linkLibs.rsp

# Rule to build all files generated by this target.
CMakeFiles/MTrees.dir/build: tmp/MTrees.exe
.PHONY : CMakeFiles/MTrees.dir/build

CMakeFiles/MTrees.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/MTrees.dir/cmake_clean.cmake
.PHONY : CMakeFiles/MTrees.dir/clean

CMakeFiles/MTrees.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" C:/GH_Local/M-trees C:/GH_Local/M-trees C:/GH_Local/M-trees/build C:/GH_Local/M-trees/build C:/GH_Local/M-trees/build/CMakeFiles/MTrees.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/MTrees.dir/depend

