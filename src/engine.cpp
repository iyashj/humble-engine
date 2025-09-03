#include "engine.h"

bool Engine::init() {
    return true;
}

void Engine::setWindowTitle(const std::string &title) {
    windowTitle = title;
}

const std::string & Engine::getWindowTitle() {
    return windowTitle;
}

