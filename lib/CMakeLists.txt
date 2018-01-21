cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
        sources/Serializer/Outputs.hpp
        sources/Serializer/Serializable.hpp
        sources/Network/ISocket.hpp
        sources/Network/Socket.hpp
        sources/Network/Network.hpp
        sources/Network/Network.cpp
        sources/Network/AsyncSocket.hpp
        sources/Network/AsyncSocket.cpp
        ${CMAKE_SOURCE_DIR}/include/Packets.hpp
        )

if (WIN32)
    set (SOURCE_FILES
            ${SOURCE_FILES}
            sources/Network/WindowsSockets.hpp
            sources/Network/WindowsSockets.cpp
            )
elseif (UNIX)
    set (SOURCE_FILES
            ${SOURCE_FILES}
            sources/Network/UnixSockets.hpp
            sources/Network/UnixTCPSockets.cpp
            sources/Network/UnixUDPSockets.cpp
            )
endif()

include_directories(sources)
include_directories(${PROJECT_SOURCE_DIR}/utils)

add_library(mutilsNet SHARED ${SOURCE_FILES})

target_link_libraries(fender)

install(TARGETS mutilsNet
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib)
