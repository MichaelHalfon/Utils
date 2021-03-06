cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -g3")

set(SOURCE_FILES
        sources/Serializer/Outputs.hpp
        sources/Serializer/Serializable.hpp
        sources/Network/ISocket.hpp
        sources/Network/Socket.hpp
        sources/Network/Network.hpp
        sources/Network/Network.cpp
        sources/Network/AsyncSocket.hpp
        sources/Network/AsyncSocket.cpp
        )

if (WIN32)
    set (SOURCE_FILES
            ${SOURCE_FILES}
            sources/Network/WindowsSockets.hpp
            sources/Network/WindowsTCPSockets.cpp
            sources/Network/WindowsUDPSockets.cpp
            )
elseif (UNIX)
    set (SOURCE_FILES
            ${SOURCE_FILES}
            sources/Network/UnixSockets.hpp
            sources/Network/UnixTCPSockets.cpp
            sources/Network/UnixUDPSockets.cpp
            sources/Network/DataStructures.hpp sources/Network/NetworkEvents.hpp)
endif()

include_directories(sources)
include_directories(${PROJECT_SOURCE_DIR}/utils)
include_directories(${PROJECT_SOURCE_DIR}/projects/fender/include)

add_library(mutilsNet STATIC ${SOURCE_FILES})

target_link_libraries(fender)

#install(TARGETS mutilsNet
#        RUNTIME DESTINATION bin
#        LIBRARY DESTINATION lib)
