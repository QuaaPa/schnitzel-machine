#include "core/Vulkan/VulkanManager.hh"
#include "core/Vulkan/VulkanContext.hh"
#include <vulkan/vulkan_core.h>

void CORE::VulkanManager::init(const char* appName, GLFWwindow* pwindow) {
    m_vulkanContext.createInstance();
    m_vulkanContext.setupDebugMessenger();
    m_vulkanContext.createSurface(pwindow);
    m_vulkanContext.pickPhysicalDevice();
    m_vulkanContext.createLogicalDevice();
}

void CORE::VulkanManager::destroy() {
    m_vulkanContext.destroy();
}
