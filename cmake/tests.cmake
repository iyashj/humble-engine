# tests.cmake - Modular test integration for HumbleEngine

include(CTest)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_engine.cpp")
    find_package(Catch2 CONFIG REQUIRED)
    add_executable(tests_engine tests/test_engine.cpp)
    target_include_directories(tests_engine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries(tests_engine PRIVATE Catch2::Catch2WithMain)
    add_test(NAME engine_tests COMMAND tests_engine)
endif()
