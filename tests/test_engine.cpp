#include <catch2/catch_test_macros.hpp>
#include "engine_core.h"

TEST_CASE("EngineCore::init initialises the engine successfully", "[engine]")
{
    auto& engienCore = EngineCore::getInstance();
    REQUIRE(engineCore.initialize() == true);
    engineCore.shutdown();
}
