#include <iostream>
#include "engine_core.h"

int main() {
    engine::EngineCore& engine = engine::EngineCore::getInstance();
    engine.initialize();
    std::cout << "Hello Humble Engine" << std::endl;
}
