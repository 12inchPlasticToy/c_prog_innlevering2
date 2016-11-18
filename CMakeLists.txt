cmake_minimum_required(VERSION 3.6)
project(Assignment_2__2016)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    main.c
    readmerge.c
    readmerge.h)

add_executable(Assignment_2__2016 ${SOURCE_FILES})