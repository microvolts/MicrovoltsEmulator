cmake_minimum_required(VERSION 3.22.1)
project(Microvolts-Emulator-V2)
enable_language(C CXX ASM)

set(CMAKE_CXX_STANDARD 20)

include_directories(PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
include_directories(PUBLIC ${CMAKE_CURRENT_LIST_DIR})
include_directories(PUBLIC ${CMAKE_SOURCE_DIR}/ExternalLibraries)

file(GLOB_RECURSE MAIN_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
add_executable(MainServer ${MAIN_SOURCES})

target_link_libraries(MainServer
        Common
        SQLiteCpp
        unofficial::sqlite3::sqlite3
        cryptopp::cryptopp
)

# Add GCC-specific flag for MSVC-like bit-field layout (only for GCC/Clang)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(MainServer PRIVATE
            -mms-bitfields       # Use MSVC style bit-field layout
    )
endif()

if(WIN32)
    target_link_libraries(MainServer wsock32 ws2_32)
endif()

set_property(TARGET MainServer PROPERTY POSITION_INDEPENDENT_CODE ON)
set_target_properties(MainServer PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})