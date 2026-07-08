#ifndef SM_CORE_CORE_H_
#define SM_CORE_CORE_H_

#include <vulkan/vulkan.h>
#include "window.h"

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

#endif // SM_CORE_CORE_H_
