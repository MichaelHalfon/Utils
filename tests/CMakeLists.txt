cmake_minimum_required(VERSION 3.8)

#if(UNIX)
#set(SOURCES TestsSer.cpp TestsNet.cpp Tests.hpp)

#include_directories(../lib/sources)
#include_directories(${PROJECT_SOURCE_DIR}/utils)
#include_directories(include/)

#add_executable(mutilsTests ${SOURCES})

#target_link_libraries(mutilsTests criterion mutilsNet fender)
#elseif(WIN32)
set(SOURCES main.cpp)

include_directories(../lib/sources)
include_directories(${PROJECT_SOURCE_DIR}/utils)
include_directories(${FENDER_ROOT}/include)
include_directories(include/)
add_executable(mutilsTests ${SOURCES})
set_target_properties(mutilsTests
        PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${MUTILS_ROOT}/tests)
if (UNIX)
    target_link_libraries(mutilsTests pthread)
endif()
target_link_libraries(mutilsTests mutilsNet fender)
#endif()
