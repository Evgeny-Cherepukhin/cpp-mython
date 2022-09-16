# Сборка итогового проекта 15 спринта (Интерпритатор языка Mython)

cmake_minimum_required(VERSION 3.11)

project(Mython CXX)
set(CMAKE_CXX_STANDARD 17)

# Переменная для создания цели. В отдельной папке хранятся тесты. 
set(MYTHON_FILES lexer.h lexer.cpp parse.h parse.cpp runtime.h runtime.cpp statement.h statement.cpp main.cpp tests/test_runner.h tests/lexer_test_open.cpp tests/parse_test.cpp tests/statement_test.cpp tests/runtime_test.cpp)

            


if(CMAKE_SYSTEM_NAME MATCHES "^MINGW")
    set(SYSTEM_LIBS -lstdc++)    
else()
    set(SYSTEM_LIBS)    
endif()

add_executable(Mython ${MYTHON_FILES})
