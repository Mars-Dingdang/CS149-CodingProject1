cmake_minimum_required(VERSION 3.10)
project(KMeans)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -fPIC")

# Include directories
include_directories(../common)

# Source files
set(SOURCES
    main.cpp
    kmeansThread.cpp
    utils.cpp
    ../common/ppm.cpp
    ../common/tasksys.cpp
)

# Add executable
add_executable(kmeans ${SOURCES})

# Link libraries
target_link_libraries(kmeans pthread)