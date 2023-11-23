cmake_minimum_required(VERSION 3.27)
project(smq)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(smq main.cpp)