cmake_minimum_required(VERSION 3.8)
project(mutilsTests)

set(SOURCES TestsSer.cpp TestsNet.cpp Tests.hpp)

include_directories(${CMAKE_SOURCE_DIR}/lib/sources)
include_directories(${CMAKE_SOURCE_DIR}/lib/fender/include/)

add_executable(mutilsTests ${SOURCES})

target_link_libraries(mutilsTests criterion mutilsNet)

