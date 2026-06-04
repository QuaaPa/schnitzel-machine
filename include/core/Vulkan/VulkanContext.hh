#ifndef VULKAN_CONTEXT
#define VULKAN_CONTEXT

#include <vulkan/vulkan_core.h>

struct VulkanContext {
    VkInstance               instance       = VK_NULL_HANDLE;
    VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;
    VkSurfaceKHR             surface        = VK_NULL_HANDLE;
    VkPhysicalDevice         physcialDevice = VK_NULL_HANDLE;
    VkDevice                 device         = VK_NULL_HANDLE;
    VkQueue                  graphicsQueue  = VK_NULL_HANDLE;
    VkQueue                  presentQueue   = VK_NULL_HANDLE;    
};
#endif // VULKAN_CONTEXT
