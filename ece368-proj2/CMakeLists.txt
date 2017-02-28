cmake_minimum_required(VERSION 3.6)
project(ece368_proj2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    07_19/constant.h
    07_19/encode.c
    07_19/encode.h
    07_19/freq.c
    07_19/freq.h
    07_19/list.c
    07_19/list.h
    07_19/main.c
    07_19/tree.c
    07_19/tree.h
    07_19/utility.c
    07_19/utility.h
    huff.c
    unhuff.c)

add_executable(ece368_proj2 ${SOURCE_FILES})