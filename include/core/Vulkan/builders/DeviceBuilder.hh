#ifndef DEVICE_BUILDER_HH
#define DEVICE_BUILDER_HH

#include <vulkan/vulkan_core.h>

struct DeviceBuilder {
    VkInstance instance;
    VkSurfaceKHR surface;

    struct Result {
        VkPhysicalDevice physicalDevice;
        VkDevice         logicalDevice;
        VkQueue          graphicsQueue;
        VkQueue          presentQueue;
        uint32_t         graphicsFamilyIndex;
        uint32_t         presentFamilyIndex;
    };
    
    Result build() const;
};

#endif // DEVICE_BUILDER_HH
