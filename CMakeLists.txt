cmake_minimum_required(VERSION 3.7)
project(mutils)

set(CMAKE_CXX_FLAGS "-W -Wall -Wextra -Werror --std=c++1z")

include_directories(${PROJECT_SOURCE_DIR}/sources/)

set(SOURCE_FILES
        main.cpp)

add_executable(mutilsTest ${SOURCE_FILES})
