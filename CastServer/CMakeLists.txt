cmake_minimum_required(VERSION 3.22.1)
project(Microvolts-Emulator-V2)
enable_language(C CXX ASM)

#set(CMAKE_C_COMPILER gcc)
#set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 20)

include_directories(PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
include_directories(PUBLIC ${CMAKE_CURRENT_LIST_DIR})
include_directories(PUBLIC ${CMAKE_SOURCE_DIR}/thirdparty/DirectXMath/Inc)

#include_directories(PUBLIC ${CMAKE_SOURCE_DIR}/thirdparty/asio-1.30.2/include)

file(GLOB_RECURSE CAST_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
add_executable(CastServer ${CAST_SOURCES})

target_link_libraries(CastServer ${Boost_LIBRARIES} Common)
#if(WIN32)
#    target_link_libraries(Common wsock32 ws2_32)
#endif()

set_property(TARGET CastServer PROPERTY POSITION_INDEPENDENT_CODE ON)
set_target_properties(CastServer PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})