#ifndef VULKAN_WINDOW_SURFACE_HH
#define VULKAN_WINDOW_SURFACE_HH

#include "VulkanInstance.hh"
#include "vulkan/vulkan.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <vulkan/vulkan_core.h>

namespace CORE {
    class VulkanInstance;

    class WindowSurface {
    private:
        VulkanInstance* m_pinstance = nullptr;
        VkSurfaceKHR m_surface = VK_NULL_HANDLE;
    public:
        WindowSurface();

        void init(VulkanInstance &instance, GLFWwindow *window);
        VkSurfaceKHR getSurface() const;
        void destroy();    
    
    };
}

#endif // VULKAN_WINDOW_SURFACE_HH
