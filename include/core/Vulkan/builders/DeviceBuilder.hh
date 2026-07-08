#ifndef DEVICE_BUILDER_HH
#define DEVICE_BUILDER_HH

#include <vulkan/vulkan_core.h>

struct DeviceBuilder {
    VkInstance instance = VK_NULL_HANDLE;
    VkSurfaceKHR surface = VK_NULL_HANDLE;

    struct Result {
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice         logicalDevice = VK_NULL_HANDLE;
        VkQueue          graphicsQueue = VK_NULL_HANDLE;
        VkQueue          presentQueue = VK_NULL_HANDLE;
        uint32_t         graphicsFamilyIndex;
        uint32_t         presentFamilyIndex;
    };
    
    Result build() const;
};

#endif // DEVICE_BUILDER_HH
