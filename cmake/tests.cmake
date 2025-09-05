# tests.cmake - Modular test integration for HumbleEngine

# Only include tests if explicitly requested
if(HUMBLEENGINE_BUILD_TESTS)
    include(CTest)
    enable_testing()

    # First try Conan-provided package
    find_package(Catch2 CONFIG QUIET)

    # Fallback: FetchContent if not provided (keeps tests optional for consumers)
    if(NOT Catch2_FOUND)
        message(STATUS "Catch2 not found. Attempting to fetch from GitHub...")
        include(FetchContent)
        FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG v3.5.2
            GIT_SHALLOW TRUE
        )
        FetchContent_MakeAvailable(Catch2)
        # Catch2 provides CMake modules in extras for older patterns (still harmless)
        if(EXISTS ${catch2_SOURCE_DIR}/extras)
            list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
        endif()
    endif()

    if(TARGET Catch2::Catch2WithMain AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_engine.cpp")
        add_executable(tests_engine tests/test_engine.cpp)
        target_include_directories(tests_engine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
        target_link_libraries(tests_engine PRIVATE Catch2::Catch2WithMain ${PROJECT_NAME})
        
        # Set C++ standard for test executable
        target_compile_features(tests_engine PRIVATE cxx_std_23)
        
        # Add the test to CTest
        add_test(NAME engine_tests COMMAND tests_engine)
        
        # Include Catch2's test discovery if available
        if(TARGET Catch2::Catch2)
            include(Catch)
            catch_discover_tests(tests_engine)
        endif()
    else()
        message(STATUS "Catch2 still unavailable – skipping test target.")
    endif()
endif()
