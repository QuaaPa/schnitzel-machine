#ifndef SM_CORE_CORE_H_
#define SM_CORE_CORE_H_

#include <vulkan/vulkan.h>

#include "window.h"
#include "Vulkan/VulkanManager.h"

namespace sm {
    class Engine {
    private:
        Engine() {};

        Window* m_pwindow = nullptr;
        VulkanManager m_vulkanManager;    
    public:
        static Engine* getInstance() {
            static Engine instance;
            return &instance;
        }

        void run(int argc, char *argv[]);
    
    private:
        void init();
    
        void mainLoop();
        void cleanup();

        Engine(const Engine&) = delete;
        Engine& operator=(const Engine&) = delete;
    };    
}

#endif // SM_CORE_CORE_H_
