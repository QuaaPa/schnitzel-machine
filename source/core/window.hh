#ifndef WINDOW_HH
#define WINDOW_HH

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Vulkan/VulkanManager.hh"

namespace CORE {


    class Window {
    private:
        GLFWwindow *m_pwindow;

        VulkanManager vulkanManager;    
    
        int m_width;
        int m_height;
    
    public:
        Window(int width, int height, const char* title);
    
        int shouldClose();
        void pollEvents();    
        void destroy();

        Window(const Window &other) = delete;
        Window(Window &&other) = delete;
        Window &operator=(const Window &other) = delete;
        Window &operator=(Window &&other) = delete;
    
    };
}
#endif // WINDOW_HH
