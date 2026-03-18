cmake_minimum_required(VERSION 3.10)
project(ParallelComputingProjects)

add_subdirectory(asst1/prog1_mandelbrot_threads)
add_subdirectory(asst1/prog2_vecintrin)
add_subdirectory(asst1/prog3_mandelbrot_ispc)
add_subdirectory(asst1/prog4_sqrt)
add_subdirectory(asst1/prog5_saxpy)
# add_subdirectory(asst1/prog6_kmeans)  # Commented out due to concrt.h dependency issue on MinGW