#ifndef ENGINE_H
#define ENGINE_H
#include <string>

class Engine {
public:
    bool init();
    void setWindowTitle(const std::string& title);
    const std::string& getWindowTitle();

private:
    std::string windowTitle;
};
#endif // ENGINE_H
