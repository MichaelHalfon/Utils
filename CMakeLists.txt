cmake_minimum_required(VERSION 3.7)
project(MUtils)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-W -Wall -Wextra -Werror")

include_directories(${PROJECT_SOURCE_DIR}/sources/)

set(SOURCE_FILES
        main.cpp)

add_executable(MUtils ${SOURCE_FILES})