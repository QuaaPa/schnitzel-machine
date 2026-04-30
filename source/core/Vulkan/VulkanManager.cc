#include "VulkanManager.hh"
#include "../../utils/DebugUtils.hh"
#include <stdexcept>
#include <vulkan/vulkan_core.h>

CORE::VulkanManager::VulkanManager() {
    /* DO NOTHING */
}

void CORE::VulkanManager::init(const char* appName, GLFWwindow* window) {
    m_instance.init(appName, 0, 0, 1);
#ifndef NDEBUG
    setupDebugMessanger();    
#endif
    m_surface.init(m_instance, window);
}

void CORE::VulkanManager::destroy() {
#ifndef NDEBUG
    DestroyDebugUtilsMessengerEXT(m_instance.getInstance(), m_debugMessanger, nullptr);
#endif
    m_surface.destroy();
    m_instance.destroy();
}

#ifndef NDEBUG


VkResult CORE::VulkanManager::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {

    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

    m_messengerInfo = {
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
        .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
        .pfnUserCallback = dbg::debugCallback,
        .pUserData = nullptr
    };

    
    if (func != nullptr) {
        return func(instance, &m_messengerInfo, pAllocator, pDebugMessenger);
    }
    else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void CORE::VulkanManager::DestroyDebugUtilsMessengerEXT(
                                                  VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
                                                  const VkAllocationCallbacks* pAllocator) {
    
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

void CORE::VulkanManager::setupDebugMessanger() {
    VkDebugUtilsMessengerCreateInfoEXT MessengerCallbackInfo;
    dbg::populateDebugMessengerCreateInfo(MessengerCallbackInfo);

    if(CreateDebugUtilsMessengerEXT(m_instance.getInstance(), nullptr, &m_debugMessanger) != VK_SUCCESS) {
        throw std::runtime_error("failed to set up debug messenger!");
    }
}
#endif // NDEBUG
