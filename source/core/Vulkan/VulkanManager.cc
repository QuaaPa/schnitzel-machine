#include "core/Vulkan/VulkanManager.hh"
#include "core/Vulkan/VulkanContext.hh"
#include <vulkan/vulkan_core.h>

void CORE::VulkanManager::init(const char* appName, GLFWwindow* window) {
    m_vulkanContext.createInstance();
    m_vulkanContext.setupDebugMessenger();
    m_vulkanContext.pickPhysicalDevice();    
}

void CORE::VulkanManager::destroy() {
    m_vulkanContext.destroy();
}
