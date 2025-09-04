#include "engine.h"

#include "logger.h"

bool Engine::init() {

    auto& logger = engine::Logger::getInstance();
    logger.debug("sample-debug-level-log");
    logger.info("sample-info-level-log");
    logger.error("sample-error-level-log");

    return true;
}

void Engine::setWindowTitle(const std::string &title) {
    windowTitle = title;
}

const std::string & Engine::getWindowTitle() {
    return windowTitle;
}

