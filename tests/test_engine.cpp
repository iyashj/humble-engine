#include <catch2/catch_test_macros.hpp>
#include "core/engine_core.h"

TEST_CASE("EngineCore::initialize initializes the engine successfully", "[engine]")
{
    auto& engineCore = engine::EngineCore::getInstance();
    REQUIRE(engineCore.initialize() == true);
    engineCore.shutdown();
}
