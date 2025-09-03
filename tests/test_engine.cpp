#include <catch2/catch_test_macros.hpp>
#include <engine.h>

TEST_CASE("Engine::init initialises the engine successfully", "[engine]")
{
    Engine engine;
    REQUIRE(engine.init() == true);
}

TEST_CASE("Engine::setWindowTitle sets the windowTile", "[engine]") {
    Engine engine;
    engine.setWindowTitle("test-title");
    REQUIRE(engine.getWindowTitle() == "test-title");
}
