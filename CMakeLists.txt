cmake_minimum_required(VERSION 3.20)
project(Graph_algorithm)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
set(CMAKE_CXX_FLAGS "-fopenmp")
#add_compile_definitions(OPENMP)


add_executable(pagerank_sync
        apps_sync/pagerank_sync.cpp timer.cpp)

add_executable(bfs_sync
        apps_sync/bfs_sync.cpp timer.cpp)

add_executable(sssp_sync
        apps_sync/sssp_sync.cpp timer.cpp)

add_executable(cc_sync
        apps_sync/cc_sync.cpp timer.cpp)

add_executable(pagerank_async
        apps_async/pagerank_async.cpp timer.cpp)

add_executable(bfs_async
        apps_async/bfs_async.cpp timer.cpp)

add_executable(sssp_async
        apps_async/sssp_async.cpp timer.cpp)

add_executable(cc_async
        apps_async/cc_async.cpp timer.cpp)

add_executable(weighted_generator
        weighted_generator.cpp
        global.h)