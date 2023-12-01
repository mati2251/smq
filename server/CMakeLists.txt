cmake_minimum_required(VERSION 3.27)
project(smq)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(smq main.cpp)
# add_custom_target(cpplint COMMAND cpplint --exclude=server/build --recursive)
target_compile_options(smq PRIVATE -Wall -Wextra -Werror)