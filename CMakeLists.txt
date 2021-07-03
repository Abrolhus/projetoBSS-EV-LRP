set (CMAKE_CXX_STANDARD 11)
cmake_minimum_required(VERSION 2.8)
project(bss)
set(SOURCES
        Graph/Edge.cpp
        Graph/Graph.cpp
        Graph/Node.cpp solution.cpp Solution.h algorithm.cpp Problem.cpp Problem.h
        main.cpp algorithm.h)
        #utils.cpp)
add_executable(run ${SOURCES})
