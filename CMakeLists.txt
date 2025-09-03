cmake_minimum_required(VERSION 4.0)

project(HumbleEngine
        VERSION 1.0.0
        DESCRIPTION "Cross-platform game engine"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_BUILD_TYPE_INIT "Release")
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

# Cross-platform compiler detection
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(COMPILER_IS_GCC_LIKE TRUE)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(COMPILER_IS_MSVC TRUE)
endif()

add_executable(humble_engine main.cpp)
