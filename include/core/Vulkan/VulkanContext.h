#ifndef SM_CORE_VULKAN_VULKANCONTEXT_H_
#define SM_CORE_VULKAN_VULKANCONTEXT_H_

#include <vulkan/vulkan_core.h>

struct VulkanContext {
    VkInstance               instance       = VK_NULL_HANDLE;
    VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;
    VkSurfaceKHR             surface        = VK_NULL_HANDLE;
    VkPhysicalDevice         physcialDevice = VK_NULL_HANDLE;
    VkDevice                 logicalDevice  = VK_NULL_HANDLE;
    VkQueue                  graphicsQueue  = VK_NULL_HANDLE;
    VkQueue                  presentQueue   = VK_NULL_HANDLE;    
};
#endif // SM_CORE_VULKAN_VULKANCONTEXT_H_
