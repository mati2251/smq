cmake_minimum_required(VERSION 3.20)
project(smq)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(files 
    src/event-loop/event-loop.cpp 
    src/actions/accept-action/accept-action.cpp 
    src/actions/client-read-action/client-read-action.cpp
    src/actions/client-write-action/client-write-action.cpp
    src/handlers/request-handler/request-handler.cpp
    src/handlers/action-handler/action-handler.cpp
    src/handlers/message-handler/message-handler.cpp
    src/request-util/request-util.cpp
    src/request-util/deserialization/deserialization.cpp
    src/request-util/serialization/serialization.cpp
    src/server-state/server-state.cpp
    src/topic/topic.cpp
    src/handlers/response-handler/response-handler.cpp
    src/configure/configure.cpp
)

include_directories(server)
Include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.4.0
)

FetchContent_MakeAvailable(Catch2)

include(CTest)
include(Catch)

function(add_test_and_executable src name)
    add_executable(${name} ${src} ${files})
    target_link_libraries(${name} PRIVATE Catch2::Catch2WithMain)
    add_test(NAME ${name} COMMAND ${name})
endfunction()


add_executable(smq src/main.cpp ${files})
target_compile_options(smq PRIVATE -Wall -Wextra -Werror)



add_test_and_executable(tests/deserialization/deserialization-test.cpp deserialization-tests)
add_test_and_executable(tests/serialization/serialization-test.cpp serialization-tests)
