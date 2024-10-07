cmake_minimum_required(VERSION 3.22.1)
project(Microvolts-Emulator-V2)
enable_language(C CXX ASM)
add_definitions(-w)
IF (WIN32)
    set(VCPKG_PLATFORM "x64-windows")
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        set(CMAKE_CXX_FLAGS "-D_WIN32_WINNT=0x0601")  # MSVC-compatible flags only
    else()
        set(CMAKE_CXX_FLAGS "-D_WIN32_WINNT=0x0601 -Wa,-mbig-obj")  # GCC/Clang-specific flag
    endif()
ELSE()
    set(VCPKG_PLATFORM "x64-linux")
    set(CMAKE_EXECUTABLE_SUFFIX ".elf")
    include_directories(PUBLIC ${CMAKE_SOURCE_DIR}/sal)
    find_package(Boost 1.74.0 COMPONENTS system)
ENDIF()

set(CMAKE_TOOLCHAIN_FILE PUBLIC ${CMAKE_SOURCE_DIR}/ExternalLibraries/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")
set(CMAKE_PREFIX_PATH PUBLIC ${CMAKE_SOURCE_DIR}/ExternalLibraries/vcpkg/vcpkg_installed/${VCPKG_PLATFORM}/share)
set(Boost_INCLUDE_DIRS PUBLIC ${CMAKE_SOURCE_DIR}/ExternalLibraries/vcpkg/vcpkg_installed/${VCPKG_PLATFORM}/include/boost)
include_directories(PUBLIC ${CMAKE_SOURCE_DIR}/ExternalLibraries/vcpkg/vcpkg_installed/${VCPKG_PLATFORM}/include/boost)
include_directories(PUBLIC ${CMAKE_SOURCE_DIR}/ExternalLibraries/vcpkg/vcpkg_installed/${VCPKG_PLATFORM}/include)
include_directories(PUBLIC ${CMAKE_SOURCE_DIR}/ExternalLibraries/vcpkg/vcpkg_installed/${VCPKG_PLATFORM}/include/SQLiteCpp)

find_package(SQLiteCpp CONFIG REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(cryptopp CONFIG REQUIRED)
find_package(directxmath CONFIG REQUIRED)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

include_directories(PUBLIC ${Boost_INCLUDE_DIRS})
include_directories(PUBLIC ${CMAKE_SOURCE_DIR}/ExternalLibraries)

# COMMON LIBRARY
add_subdirectory(Common)
include_directories(${CMAKE_CURRENT_LIST_DIR}/CommonLib)

# AUTH SERVER
add_subdirectory(AuthServer)

# CAST SERVER
add_subdirectory(CastServer)

# MAIN SERVER
add_subdirectory(MainServer)
