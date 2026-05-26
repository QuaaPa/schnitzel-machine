#include "VulkanWindowSurface.hh"
#include "VulkanInstance.hh"
#include "VulkanManager.hh"
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

void CORE::WindowSurface::init(VulkanInstance &instance, GLFWwindow *window) {
    if(glfwCreateWindowSurface(instance.getInstance(), window, nullptr, &m_surface) != VK_SUCCESS)    {
        throw std::runtime_error("failed to create surface!");
    }
}

VkSurfaceKHR CORE::WindowSurface::getSurface() const {
    return m_surface;
}

void CORE::WindowSurface::destroy(VulkanInstance &instance) {
	if (m_surface != VK_NULL_HANDLE)
        {
            vkDestroySurfaceKHR(instance.getInstance(), m_surface, nullptr);
            m_surface = VK_NULL_HANDLE;
        }  
}
