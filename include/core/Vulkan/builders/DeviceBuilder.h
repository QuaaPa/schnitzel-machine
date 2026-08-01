#ifndef SM_CORE_VULKAN_BUILDERS_DEVICEBUILDER_H_
#define SM_CORE_VULKAN_BUILDERS_DEVICEBUILDER_H_

#include <vulkan/vulkan_core.h>

struct DeviceBuilder {
private:
    struct Result {
        VkPhysicalDevice physicalDevice;
        VkDevice logicalDevice;

        VkQueue graphicsQueue;
        VkQueue presentQueue;
        // VkQueue transferQueue;
        uint32_t graphicsFamilyIndex;
        uint32_t presentFamilyIndex;
        // uint32_t transferFamilyIndex;

    };    
    
public:
    VkInstance instance = VK_NULL_HANDLE;
    VkSurfaceKHR surface = VK_NULL_HANDLE;

    
    [[nodiscard]] Result build() const;
};

#endif // SM_CORE_VULKAN_BUILDERS_DEVICEBUILDER_H_
