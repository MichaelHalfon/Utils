cmake_minimum_required(VERSION 3.8)
project(mutils_lib)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
        sources/Network/ISocket.hpp
        sources/Network/INetwork.hpp
        sources/Serializer/Outputs.hpp
        sources/Serializer/Serializable.hpp sources/Network/UnixSockets.cpp sources/Network/UnixSockets.hpp)

add_library(mutils_lib SHARED ${SOURCE_FILES})

install(TARGETS mutils_lib
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib)


