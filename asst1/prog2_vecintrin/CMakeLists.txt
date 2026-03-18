cmake_minimum_required(VERSION 3.10)
project(VecIntrin)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall")

# Include directories
include_directories(../common)

# Source files
set(SOURCES
    main.cpp
    CS149intrin.cpp
    logger.cpp
)

# Add executable
add_executable(myexp ${SOURCES})

# Link libraries
target_link_libraries(myexp pthread)