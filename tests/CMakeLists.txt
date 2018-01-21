cmake_minimum_required(VERSION 3.8)

set(SOURCES TestsSer.cpp TestsNet.cpp Tests.hpp)

include_directories(../lib/sources)
include_directories(${PROJECT_SOURCE_DIR}/utils)

add_executable(mutilsTests ${SOURCES})

target_link_libraries(mutilsTests criterion mutilsNet fender)

