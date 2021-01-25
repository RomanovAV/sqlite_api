cmake_minimum_required(VERSION 3.17)
project(DBApi)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -Wall)

include_directories(sqlite-amalgamation-3340100)

add_executable(DBApi
        sqlite-amalgamation-3340100/sqlite3.c
        sqlite-amalgamation-3340100/sqlite3.h
        main.cpp
        )