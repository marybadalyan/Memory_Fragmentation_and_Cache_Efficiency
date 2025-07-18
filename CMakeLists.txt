cmake_minimum_required(VERSION 3.10)
project(main CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define the output directory for the analysis files
set(ANALYSIS_DIR "${CMAKE_BINARY_DIR}/analysis")
file(MAKE_DIRECTORY ${ANALYSIS_DIR})

# Define the executable first
if (WIN32)
  add_executable(main_windows main_windows.cpp)
else()
  add_executable(main_linux main_linux.cpp)
endif()


add_executable(arena_allocator arena_allocator.cpp)
add_executable(slab_allocator slab_allocator.cpp)
add_executable(pool_allocator pool_allocator.cpp)
# could use perf tools for more detailed analysis but that is linux specific