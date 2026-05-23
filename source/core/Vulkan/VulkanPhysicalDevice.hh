#ifndef VULKAN_PHYSICAL_DEVICE_HH
#define VULKAN_PHYSICAL_DEVICE_HH

#include "VulkanInstance.hh"
#include "vulkan/vulkan.h"

namespace CORE {
    class VulkanInstance;
    class PhysicalDevice {
    private:
        VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
        
    public:      
        VkPhysicalDevice pickPhysicalDevice(VulkanInstance &instance);
        
    private:
        bool isDeviceSuitable(VkPhysicalDevice device);
    };
}
#endif // VULKAN_PHYSICAL_DEVICE_HH
