#include <catch2/catch_test_macros.hpp>
#include "core/engine_core.h"
#include "logging/logger.h"
#include "rendering/renderer.h"

TEST_CASE("EngineCore initialization", "[engine][core]")
{
    auto& engineCore = engine::EngineCore::getInstance();
    REQUIRE(engineCore.initialize() == true);
    engineCore.shutdown();
}

TEST_CASE("Logger functionality", "[engine][logging]")
{
    SECTION("Logger initialization") {
        // Test logger initialization - should not crash
        REQUIRE_NOTHROW(engine::Logger::initialize());
        REQUIRE_NOTHROW(engine::Logger::shutdown());
    }
    
    SECTION("Logger macros work") {
        // Test logging macros - should not crash
        REQUIRE_NOTHROW(LOG_INFO_MSG("Test info message"));
        REQUIRE_NOTHROW(LOG_WARN_MSG("Test warning message"));
        REQUIRE_NOTHROW(LOG_ERROR_MSG("Test error message"));
    }
}

TEST_CASE("Renderer basic functionality", "[engine][rendering]")
{
    SECTION("Renderer creation and basic operations") {
        engine::Renderer renderer;
        
        // Test renderer initialization
        auto result = renderer.initialize(800, 600, "Test Window");
        REQUIRE(result.has_value());
        
        // Test basic properties
        REQUIRE(renderer.getWindowWidth() == 800);
        REQUIRE(renderer.getWindowHeight() == 600);
        
        renderer.shutdown();
    }
}
