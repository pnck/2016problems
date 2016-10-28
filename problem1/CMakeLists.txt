cmake_minimum_required(VERSION 3.2)
project(baseCJK)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#add_definitions(-D_USEASM)
add_definitions(-D_WITH_ENCODE)
#add_definitions(-D_WITH_DECODE)
set(SOURCE_FILES main.cpp basecjk.cpp basecjk.h)
add_executable(baseCJK ${SOURCE_FILES})