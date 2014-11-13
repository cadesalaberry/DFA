cmake_minimum_required(VERSION 2.8.4)
project(banana)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

set(SOURCE_FILES src/main.cpp)
add_executable(banana ${SOURCE_FILES})