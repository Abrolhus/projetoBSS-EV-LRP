set (CMAKE_CXX_STANDARD 11)
cmake_minimum_required(VERSION 2.8)
project(bss)
set(SOURCES
        Edge.cpp
        Graph.cpp
        main.cpp
        Node.cpp
        )
add_executable(run ${SOURCES})
