cmake_minimum_required(VERSION 3.7)
project(mutils)

set(CMAKE_CXX_FLAGS "-W -Wall -Wextra -Werror --std=c++1z")

include_directories(${PROJECT_SOURCE_DIR}/sources/)

set(SERIALIZER_DIRECTORY ${PROJECT_SOURCE_DIR}/sources/serializer)

set(serializerSRCS ${SERIALIZER_DIRECTORY}/Outputs.hpp ${SERIALIZER_DIRECTORY}/Serializable.hpp)

set(SOURCE_FILES
        main.cpp)

#add_library(mutils SHARED ${fakeSRCS} ${serializerSRCS})

add_executable(mutilsTest ${SOURCE_FILES})
