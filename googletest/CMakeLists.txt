project(google_tests)

add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

include_directories(../inc)
include_directories(../)
set(SOURCES ${DBApi_SOURCE_DIR}/test/connector_test.cpp ../test/query_test.cpp)

set(PROJ_SOURCES
        ${DBApi_SOURCE_DIR}/src/connector.cpp
        ${DBApi_SOURCE_DIR}/src/query.cpp ../src/error_handler.cpp ../inc/error_handler.h ../src/db_interface.cpp)

set(PROJ_INCLUDES
        ${DBApi_SOURCE_DIR}/inc/connector.h
        ${DBApi_SOURCE_DIR}/sqlite-amalgamation-3340100/sqlite3.c
        ${DBApi_SOURCE_DIR}/sqlite-amalgamation-3340100/sqlite3.h
        ${DBApi_SOURCE_DIR}/inc/query.h ../inc/db_interface.h)

add_executable(google_Tests_run
        ${SOURCES}
        ${PROJ_SOURCES}
        ${PROJ_INCLUDES}
        )

target_link_libraries(google_Tests_run gtest gtest_main)