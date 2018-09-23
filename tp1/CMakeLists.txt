cmake_minimum_required(VERSION 3.10)
project(tp1)

set(CMAKE_CXX_STANDARD 11)

include_directories(src)

add_executable(tp1
        src/Matrix.h
        src/Parser.h
        src/tp1.cpp)
