# tests.cmake - Modular test integration for HumbleEngine

if(HUMBLEENGINE_BUILD_TESTS)
    include(CTest)
    enable_testing()

    # Find Catch2 - should be available via Conan
    find_package(Catch2 3 REQUIRED)

    # Collect all test files
    file(GLOB_RECURSE TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp")
    
    if(TEST_SOURCES)
        add_executable(humble_engine_tests ${TEST_SOURCES})
        
        target_link_libraries(humble_engine_tests 
            PRIVATE 
            Catch2::Catch2WithMain
            ${PROJECT_NAME}
        )
        
        target_compile_features(humble_engine_tests PRIVATE cxx_std_23)
        
        # Include directories for tests
        target_include_directories(humble_engine_tests 
            PRIVATE 
            ${CMAKE_CURRENT_SOURCE_DIR}/include
            ${CMAKE_CURRENT_SOURCE_DIR}/src
        )

        # Discover and register tests with CTest
        include(Catch)
        catch_discover_tests(humble_engine_tests
            TEST_PREFIX "HumbleEngine::"
        )

        message(STATUS "Added ${CMAKE_MATCH_COUNT} test files to humble_engine_tests")
    else()
        message(WARNING "No test files found in tests/ directory")
    endif()
endif()
