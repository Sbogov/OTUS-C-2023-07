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
CMAKE_SOURCE_DIR = /mnt/d/project_work_OTUS/call_graph_with_graphviz

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/d/project_work_OTUS/call_graph_with_graphviz/build

# Utility rule file for graph.

# Include any custom commands dependencies for this target.
include CMakeFiles/graph.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/graph.dir/progress.make

CMakeFiles/graph:
	dot -Gsize=11,8.5 -Grankdir=LR -Tpng ../output_graphviz/call_graph.dot -o ../graph_image/call_graph.png
	cat ../output_graphviz/call_graph.dot
	eog ../graph_image/call_graph.png

graph: CMakeFiles/graph
graph: CMakeFiles/graph.dir/build.make
.PHONY : graph

# Rule to build all files generated by this target.
CMakeFiles/graph.dir/build: graph
.PHONY : CMakeFiles/graph.dir/build

CMakeFiles/graph.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/graph.dir/cmake_clean.cmake
.PHONY : CMakeFiles/graph.dir/clean

CMakeFiles/graph.dir/depend:
	cd /mnt/d/project_work_OTUS/call_graph_with_graphviz/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/d/project_work_OTUS/call_graph_with_graphviz /mnt/d/project_work_OTUS/call_graph_with_graphviz /mnt/d/project_work_OTUS/call_graph_with_graphviz/build /mnt/d/project_work_OTUS/call_graph_with_graphviz/build /mnt/d/project_work_OTUS/call_graph_with_graphviz/build/CMakeFiles/graph.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/graph.dir/depend

