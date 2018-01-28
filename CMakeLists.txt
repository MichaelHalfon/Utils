cmake_minimum_required(VERSION 3.8)

if (NOT MSVC)
    set(CMAKE_CXX_FLAGS "-Wall -Wextra --std=c++1z")
else()
    set(CMAKE_CXX_FLAGS "/std:c++latest")
endif()

add_subdirectory(lib)
#add_subdirectory(tests)
