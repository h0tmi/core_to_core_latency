cmake_minimum_required(VERSION 3.20)
project(inter-core-latency CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O3 -march=native")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

include_directories(${PROJECT_SOURCE_DIR}/include)

set(SOURCES
    src/main.cpp
    src/benchmark_engine.cpp
    src/one_cacheline_bench.cpp
    src/two_cacheline_bench.cpp
    src/statistics.cpp
    src/tsc_clock.cpp
    src/cpu_utils.cpp
)

add_executable(inter-core-latency ${SOURCES})

target_link_libraries(inter-core-latency pthread)
