cmake_minimum_required(VERSION 3.17)
project(DBApi)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -Wall)

enable_testing()

include_directories(sqlite-amalgamation-3340100)
include_directories(inc)


add_subdirectory(googletest)

add_executable(DBApi
        sqlite-amalgamation-3340100/sqlite3.c
        sqlite-amalgamation-3340100/sqlite3.h
        main.cpp
        src/connector.cpp
        inc/connector.h
        inc/query.h
        src/query.cpp src/error_handler.cpp inc/error_handler.h)