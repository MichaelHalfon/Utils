cmake_minimum_required(VERSION 3.8)
project(mutils)

if (NOT MSVC)
    set(CMAKE_CXX_FLAGS "-Wall -Wextra --std=c++1z -g3")
endif()

add_subdirectory(lib)
add_subdirectory(tests)

