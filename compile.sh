#!/bin/bash

# Define the base directory, assuming the script is run from the project root
BASE_DIR=$(pwd)

# Compile the source files
g++ -O3 -I"$BASE_DIR/headers" -c "$BASE_DIR/src/ss_method.cpp" -o "$BASE_DIR/build/ss_method.o"
g++ -O3 -I"$BASE_DIR/headers" -c "$BASE_DIR/src/cluster_utils.cpp" -o "$BASE_DIR/build/cluster_utils.o"
g++ -O3 -I"$BASE_DIR/headers" -c "$BASE_DIR/src/nearest_neighbour.cpp" -o "$BASE_DIR/build/nearest_neighbour.o"
g++ -O3 -I"$BASE_DIR/headers" -c "$BASE_DIR/src/nearest_pair.cpp" -o "$BASE_DIR/build/nearest_pair.o"

# Link the object files into an executable
g++ "$BASE_DIR/build/ss_method.o" "$BASE_DIR/build/cluster_utils.o" "$BASE_DIR/build/nearest_neighbour.o" "$BASE_DIR/build/nearest_pair.o" -o "$BASE_DIR/build/MyApp"