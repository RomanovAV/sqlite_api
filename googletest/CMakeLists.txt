project(google_tests)

add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

include_directories(../inc)
include_directories(../)
set(SOURCES ${DBApi_SOURCE_DIR}/test/connector_test.cpp)

set(PROJ_SOURCES
        ${DBApi_SOURCE_DIR}/src/connector.cpp)

set(PROJ_INCLUDES
        ${DBApi_SOURCE_DIR}/inc/connector.h
        ${DBApi_SOURCE_DIR}/sqlite-amalgamation-3340100/sqlite3.c
        ${DBApi_SOURCE_DIR}/sqlite-amalgamation-3340100/sqlite3.h)

add_executable(google_Tests_run
        ${SOURCES}
        ${PROJ_SOURCES}
        ${PROJ_INCLUDES}
        )

target_link_libraries(google_Tests_run gtest gtest_main)