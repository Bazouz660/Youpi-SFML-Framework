# Specify the minimum required version of CMake
cmake_minimum_required(VERSION 3.5)

include(FetchContent)
include(ExternalProject)

if(UNIX)
    set(CMAKE_CXX_FLAGS "-ldl")
endif()

# Clone and bootstrap vcpkg if it doesn't exist
if (NOT EXISTS ${CMAKE_BINARY_DIR}/vcpkg)
    execute_process(COMMAND git clone https://github.com/Microsoft/vcpkg.git ${CMAKE_BINARY_DIR}/vcpkg)
    if (WIN32)
        execute_process(COMMAND ${CMAKE_BINARY_DIR}/vcpkg/bootstrap-vcpkg.bat)
    elseif(UNIX)
        execute_process(COMMAND ${CMAKE_BINARY_DIR}/vcpkg/bootstrap-vcpkg.sh)
    endif()
endif()

# Install the required packages

if (WIN32)
    set(TRIPLET "x64-mingw-static")
elseif(UNIX)
    set(TRIPLET "x64-linux")
endif()

# Install system dependencies (autoconf, automake, libtool, etc.)
execute_process(COMMAND ${CMAKE_BINARY_DIR}/vcpkg/vcpkg install xcb-util-m4 --host-triplet=${TRIPLET} --triplet=${TRIPLET})

execute_process(COMMAND ${CMAKE_BINARY_DIR}/vcpkg/vcpkg install sfml --host-triplet=${TRIPLET} --triplet=${TRIPLET})

# Set the path to the vcpkg toolchain file
set(VCPKG_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build/vcpkg" CACHE STRING "")
set(VCPKG_TARGET_TRIPLET ${TRIPLET} CACHE STRING "")
set(VCPKG_HOST_TRIPLET ${TRIPLET} CACHE STRING "")
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT_DIR}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

# Set the project name
project(Youpi-Engine)

# Set the C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)

if (WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -static")
endif()

# Set the project version
set(PROJECT_VERSION_MAJOR alpha-4)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 1)
set(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

# Find SFML package
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML COMPONENTS graphics window system network audio REQUIRED)

# Add the client and server subdirectories
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(ypi)
