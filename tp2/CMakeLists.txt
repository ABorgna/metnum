cmake_minimum_required(VERSION 3.10)
project(tp2)

set(CMAKE_CXX_STANDARD 11)

include_directories(src)
include_directories(src/entry)
include_directories(src/model)

add_executable(tp2
        src/model/potencia.cpp
        src/model/potencia.h
        src/tp2.cpp)
