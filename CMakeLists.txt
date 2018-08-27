cmake_minimum_required(VERSION 3.10)
project(metnum_tp1)

set(CMAKE_CXX_STANDARD 11)

include_directories(src)

add_executable(metnum_tp1
        src/main.cpp
        src/PRMatrix.cpp
        src/PRMatrix.h)
