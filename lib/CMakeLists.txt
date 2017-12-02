cmake_minimum_required(VERSION 3.8)
project(mutils_lib)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
        sources/Network/ISocket.hpp
        sources/Network/INetwork.hpp
        sources/Serializer/Outputs.hpp
        sources/Serializer/Serializable.hpp sources/Network/UnixSockets.cpp sources/Network/UnixSockets.hpp)

include_directories(${CMAKE_SOURCE_DIR}/lib/sources)

add_library(mutilsNet SHARED ${SOURCE_FILES})

install(TARGETS mutilsNet
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib)


