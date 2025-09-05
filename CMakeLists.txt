cmake_minimum_required(VERSION 3.20)
project(HumbleEngine LANGUAGES CXX)

project(HumbleEngine
        VERSION 1.0.0
        DESCRIPTION "Cross-platform game engine"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

## Set a reasonable default build type when not specified by the user
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

# Cross-platform compiler detection
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(COMPILER_IS_GCC_LIKE TRUE)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(COMPILER_IS_MSVC TRUE)
endif()

# Options
option(HUMBLEENGINE_BUILD_SHARED "Build as shared library" OFF)
option(HUMBLEENGINE_BUILD_EXAMPLES "Build example executables" OFF)
option(HUMBLEENGINE_BUILD_TESTS "Build and run tests" OFF)
option(HUMBLEENGINE_ENABLE_LTO "Enable Link Time Optimization" ON)
option(HUMBLEENGINE_ENABLE_SANITIZERS "Enable sanitizers (Debug only)" OFF)

# Platform-specific backend configuration
if(WIN32)
    # Windows: DirectX/OpenGL backends available
    set(PLATFORM_DEFINES PLATFORM_DESKTOP GRAPHICS_API_OPENGL_33)
elseif(APPLE)
    # macOS: Metal/OpenGL backends
    set(PLATFORM_DEFINES PLATFORM_DESKTOP GRAPHICS_API_OPENGL_33)
    if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
        set(PLATFORM_DEFINES PLATFORM_IOS GRAPHICS_API_OPENGL_ES2)
    endif()
elseif(UNIX)
    # Linux: OpenGL/Vulkan backends
    set(PLATFORM_DEFINES PLATFORM_DESKTOP GRAPHICS_API_OPENGL_33)
    if(ANDROID)
        set(PLATFORM_DEFINES PLATFORM_ANDROID GRAPHICS_API_OPENGL_ES2)
    endif()
endif()

set(ENGINE_SOURCES
        src/rendering/renderer.cpp
        src/logging/logger.cpp
    src/core/engine_core.cpp
)

file(GLOB_RECURSE ENGINE_HEADERS
    "include/*.h"
)

if(HUMBLEENGINE_BUILD_SHARED)
    add_library(${PROJECT_NAME} SHARED ${ENGINE_SOURCES})
    target_compile_definitions(${PROJECT_NAME} PRIVATE HUMBLEENGINE_BUILD_SHARED)
else()
    add_library(${PROJECT_NAME} STATIC ${ENGINE_SOURCES})
endif()

add_library(HumbleEngine::HumbleEngine ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)


find_package(raylib REQUIRED)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    raylib
)

# Export the platform defines as preprocessor symbols (e.g. -DPLATFORM_DESKTOP)
if(PLATFORM_DEFINES)
    target_compile_definitions(${PROJECT_NAME} PRIVATE ${PLATFORM_DEFINES})
endif()

if(COMPILER_IS_GCC_LIKE)
    target_compile_options(${PROJECT_NAME} PRIVATE
            -Wall -Wextra -Wpedantic -Wshadow -Wconversion
            -Wunreachable-code -Wuninitialized
            $<$<CONFIG:Debug>:-g3 -O0 -fno-omit-frame-pointer>
            $<$<CONFIG:Release>:-O3 -DNDEBUG>
    )

    # Sanitizers (Debug only)
    if(HUMBLEENGINE_ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=address,undefined)
        target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address,undefined)
    endif()

elseif(COMPILER_IS_MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
            /W4 /permissive- /utf-8
            $<$<CONFIG:Debug>:/Od /Zi>
            $<$<CONFIG:Release>:/O2 /DNDEBUG>
    )

    # Enable parallel compilation
    target_compile_options(${PROJECT_NAME} PRIVATE /MP)
endif()

# Link Time Optimization (Release only)
if(HUMBLEENGINE_ENABLE_LTO)
    set_target_properties(${PROJECT_NAME} PROPERTIES
            INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE
            INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO TRUE
    )
endif()

# Examples
if(HUMBLEENGINE_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Installation (for package managers)
include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
        EXPORT HumbleEngineTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(FILES ${ENGINE_HEADERS}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/humble_engine
)

# Package configuration
install(EXPORT HumbleEngineTargets
        FILE HumbleEngineTargets.cmake
        NAMESPACE HumbleEngine::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/HumbleEngine
)

# Generate and install package config file
include(CMakePackageConfigHelpers)
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/HumbleEngineConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/HumbleEngineConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/HumbleEngine
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/HumbleEngineConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/HumbleEngineConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/HumbleEngineConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/HumbleEngine
)



# --- Tests ---
include(cmake/tests.cmake)

add_executable(humble_engine main.cpp)
target_link_libraries(humble_engine PRIVATE ${PROJECT_NAME})
