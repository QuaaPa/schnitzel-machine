#ifndef VULKAN_DEVICE_HH
#define VULKAN_DEVICE_HH

#include <cstdint>

#include <set>

#include <vulkan/vulkan_core.h>

#include "VulkanInstance.hh"
#include "VulkanWindowSurface.hh"
#include "VulkanPhysicalDevice.hh"

namespace CORE {
    class VulkanInstance;
    class WindowSurface;
    class Device {
    private:
        PhysicalDevice* m_physicalDevice = nullptr;;
        VkDevice m_device;
        VkQueue m_graphicsQueue;
        VkQueue m_presentQueue;        

        struct QueueFamilyIndices {
            uint32_t graphicsFamily = 0;;
            uint32_t presentFamily = 0;          
        };
    public:
      void init(VulkanInstance &instance, WindowSurface surface);

      void destroy();

    private:
      void findQueueFamilies(VkPhysicalDevice physicalDevice,
                             VkSurfaceKHR surface,
                             QueueFamilyIndices *pqueueFamilyIndices);
        
    };
}

#endif // VULKAN_LOGICAL_DEVICE_HH
