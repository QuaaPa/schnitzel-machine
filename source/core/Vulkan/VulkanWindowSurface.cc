#include "VulkanWindowSurface.hh"
#include "VulkanInstance.hh"
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

CORE::WindowSurface::WindowSurface() {
    /* DO NOTHING */ 
}

void CORE::WindowSurface::init(VulkanInstance &instance, GLFWwindow *window) {
    m_pinstance = &instance;
    if(glfwCreateWindowSurface(instance.getInstance(), window, nullptr, &m_surface) != VK_SUCCESS)    {
        throw std::runtime_error("failed to create surface!");
    }
}
 
VkSurfaceKHR CORE::WindowSurface::getSurface() const {
    return m_surface;
}

void CORE::WindowSurface::destroy() {
	if (m_surface != VK_NULL_HANDLE)
        {
            vkDestroySurfaceKHR(m_pinstance->getInstance(), m_surface, nullptr);
            m_surface = VK_NULL_HANDLE;
        }  
}
