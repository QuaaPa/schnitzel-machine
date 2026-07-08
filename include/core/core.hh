#ifndef CORE_HH
#define CORE_HH

#include <vulkan/vulkan.h>
#include "window.hh"

class Engine {
private:
    Engine() {};

    sm::Window* m_pwindow = nullptr;
public:
    static Engine* getInstance() {
        static Engine instance;
        return &instance;
    }

    void run(int argc, char *argv[]);
    
private:
    void initWindow();
    
    void mainLoop();
    void cleanup();

    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;
};

#endif // CORE_HH
