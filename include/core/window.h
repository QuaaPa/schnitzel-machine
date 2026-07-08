#ifndef SM_CORE_WINDOW_H_
#define SM_CORE_WINDOW_H_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Vulkan/VulkanManager.h"

namespace sm {
    class Window {
    private:
        GLFWwindow *m_pwindow;

        VulkanManager m_vulkanManager;    
    
        int m_width;
        int m_height;
    
    public:
        Window(int width, int height, const char* title);
    
        bool shouldClose();
        void pollEvents();
        void drawFrame();
        void destroy();

        Window(const Window &other) = delete;
        Window(Window &&other) = delete;
        Window &operator=(const Window &other) = delete;
        Window &operator=(Window &&other) = delete;    
    };
}
#endif // SM_CORE_WINDOW_H_
