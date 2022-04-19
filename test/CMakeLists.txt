cmake_minimum_required(VERSION 3.8)

add_executable(Lexer_test Lexer_Test.cpp)
add_executable(Parser_test Parser_Test.cpp)
add_executable(Interpreter_test Interpreter_Test.cpp)
add_executable(Service_test Service_test.cpp)

target_link_libraries(Lexer_test PUBLIC
    gtest_main
    Interpreter)

target_link_libraries(Parser_test PUBLIC
    gtest_main
    Interpreter)

target_link_libraries(Interpreter_test PUBLIC
    gtest_main
    Interpreter)

target_link_libraries(Service_test PUBLIC
    gtest_main
    Interpreter)

add_test(
    NAME Lexer_test
    COMMAND Lexer_test
)

add_test(
    NAME Parser_test
    COMMAND Parser_test
)

add_test(
    NAME Interpreter_test
    COMMAND Interpreter_test
)

add_test(
    NAME Service_test
    COMMAND Service_test
)