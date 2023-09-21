cmake_minimum_required(VERSION 3.8)

set(This Interpreter)

project(${This} C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

include_directories(include/)

set(Headers
    include/Lexer.h
    include/Parser.h
    include/Interpreter.h

    include/Nodes.h
)

set(Sources
    src/Lexer.cpp
    src/Parser.cpp
    src/Interpreter.cpp

    src/Nodes.cpp
)

add_library(${This} ${Headers} ${Sources})
add_executable(interpreter_v_0_0_1 src/Main.cpp)
target_link_libraries(interpreter_v_0_0_1 PUBLIC ${This})

enable_testing()

add_subdirectory(googletest)

add_subdirectory(test)