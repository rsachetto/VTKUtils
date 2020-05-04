cmake_minimum_required(VERSION 3.6)
project(microToVTK)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp volume.cpp volume.h)
add_executable(microToVTK ${SOURCE_FILES})