cmake_minimum_required(VERSION 3.20)

# --- Project metadata ---
project(HumbleEngine
        VERSION 1.0.0
        DESCRIPTION "Cross-platform game engine"
        HOMEPAGE_URL "https://github.com/iyashj/humble-engine"
        LANGUAGES CXX
)

include(CMakeDependentOption)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# --- Options ---
option(HUMBLEENGINE_BUILD_SHARED "Build as shared library" OFF)
option(HUMBLEENGINE_BUILD_EXAMPLES "Build example executables" OFF)
option(HUMBLEENGINE_BUILD_TESTS "Build and run tests" OFF)
option(HUMBLEENGINE_ENABLE_LTO "Enable Link Time Optimization" ON)
option(HUMBLEENGINE_ENABLE_SANITIZERS "Enable sanitizers (Debug only)" OFF)
set(HUMBLEENGINE_SANITIZERS "" CACHE STRING "List of sanitizers (address,undefined,thread,memory)")

# --- Sources ---
set(ENGINE_SOURCES
        src/rendering/renderer.cpp
        src/logging/logger.cpp
        src/core/engine_core.cpp
        src/game/game.cpp
)

# --- Library target ---
if(HUMBLEENGINE_BUILD_SHARED)
    add_library(${PROJECT_NAME} SHARED ${ENGINE_SOURCES})
    target_compile_definitions(${PROJECT_NAME} PRIVATE HUMBLEENGINE_BUILD_SHARED)
else()
    add_library(${PROJECT_NAME} STATIC ${ENGINE_SOURCES})
endif()

add_library(HumbleEngine::HumbleEngine ALIAS ${PROJECT_NAME})

# --- Single-header public API ---
target_sources(${PROJECT_NAME}
        PUBLIC
        FILE_SET HEADERS
        BASE_DIRS include
        FILES include/humble_engine.h
)

target_include_directories(${PROJECT_NAME}
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# --- Language standard ---
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_23)

# --- Dependencies ---
find_package(raylib REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC raylib)

# --- Compiler warnings (local only, not exported) ---
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE
            -Wall -Wextra -Wpedantic -Wshadow -Wconversion
            -Wunreachable-code -Wuninitialized
            $<$<CONFIG:Debug>:-g3 -O0 -fno-omit-frame-pointer>
            $<$<CONFIG:Release>:-O3 -DNDEBUG>
    )
elseif(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
            /W4 /permissive- /utf-8
            $<$<CONFIG:Debug>:/Od /Zi>
            $<$<CONFIG:Release>:/O2 /DNDEBUG>
            /MP
    )
endif()

# --- Sanitizers (Debug only) ---
if(HUMBLEENGINE_ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    foreach(sanitizer IN LISTS HUMBLEENGINE_SANITIZERS)
        target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=${sanitizer})
        target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=${sanitizer})
    endforeach()
endif()

# --- Link Time Optimization ---
if(HUMBLEENGINE_ENABLE_LTO)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT ipo_supported OUTPUT error)
    if(ipo_supported)
        set_property(TARGET ${PROJECT_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    endif()
endif()

# --- Install / Export ---
install(TARGETS ${PROJECT_NAME}
        EXPORT HumbleEngineTargets
        FILE_SET HEADERS
)

install(EXPORT HumbleEngineTargets
        FILE HumbleEngineTargets.cmake
        NAMESPACE HumbleEngine::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/HumbleEngine
)

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

# --- Examples ---
if(HUMBLEENGINE_BUILD_EXAMPLES AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/examples/CMakeLists.txt")
    add_subdirectory(examples)
endif()

# --- Tests ---
if(HUMBLEENGINE_BUILD_TESTS AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/cmake/tests.cmake")
    include(cmake/tests.cmake)
endif()

# --- Top-level executable (only if this is root project) ---
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    add_executable(humble_engine main.cpp)
    target_link_libraries(humble_engine PRIVATE ${PROJECT_NAME})
endif()
