cmake_minimum_required(VERSION 3.8)
project(mutils)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
        sources/Network/ISocket.hpp
        sources/Network/UnixSocket.cpp
        sources/Network/UnixSocket.hpp
        sources/Network/INetwork.hpp
        sources/Serializer/Outputs.hpp
        sources/Serializer/Serializable.hpp)

add_library(mutils SHARED ${SOURCE_FILES})