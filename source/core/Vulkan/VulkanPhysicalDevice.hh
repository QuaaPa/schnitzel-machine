#ifndef VULKAN_PHYSICAL_DEVICE_HH
#define VULKAN_PHYSICAL_DEVICE_HH

#include "VulkanInstance.hh"
#include "vulkan/vulkan.h"
#include <vulkan/vulkan_core.h>

namespace CORE {
    class VulkanInstance;
    class PhysicalDevice {
    private:
        VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;

    public:
        VkPhysicalDevice pickPhysicalDevice(VulkanInstance &instance);
        
    private:
        bool isDeviceSuitable(VkPhysicalDevice physicalDevice);
        bool checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice);

        const std::vector<const char*> deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };
        
    };
}
#endif // VULKAN_PHYSICAL_DEVICE_HH
