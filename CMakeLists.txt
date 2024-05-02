cmake_minimum_required(VERSION 3.10)
project(MTrees VERSION 1.0)
set(CMAKE_C_COMPILER "C:/msys64/ucrt64/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "C:/msys64/ucrt64/bin/g++.exe")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tmp)
set(CMAKE_CXX_STANDARD 17)  # Set C++17 as the minimum standard
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(headers)
file(GLOB SOURCES "src/*.cpp")


# Define the executable target based on the SOURCES variable
add_executable(${PROJECT_NAME} ${SOURCES})

# After defining the executable, set target properties
set_target_properties(${PROJECT_NAME} PROPERTIES
    LINK_FLAGS "-Wl,--verbose"
)