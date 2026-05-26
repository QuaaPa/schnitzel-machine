#include "VulkanPhysicalDevice.hh"
#include "VulkanInstance.hh"
#include <cstdint>
#include <optional>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan_core.h>

VkPhysicalDevice CORE::PhysicalDevice::pickPhysicalDevice(VulkanInstance &instance) {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance.getInstance(), &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance.getInstance(), &deviceCount, devices.data());
    
    for(const auto& device : devices) {
        if (isDeviceSuitable(device)) {
            m_physicalDevice = device;
#ifndef NDEBUG
            VkPhysicalDeviceProperties deviceProperties;
            vkGetPhysicalDeviceProperties(device, &deviceProperties);
            VkPhysicalDeviceFeatures deviceFeatures;
            vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
            std::cout << "deviceProperties.deviceName: " << deviceProperties.deviceName << std::endl;
#endif // NDEBUG
            break;
        }
    }
    
    if(m_physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
    
    return m_physicalDevice;
}

bool CORE::PhysicalDevice::isDeviceSuitable(VkPhysicalDevice device) {
    return true;
}

// bool CORE::PhysicalDevice::isDeviceSuitable(VkPhysicalDevice device) {
//     std::optional<uint32_t> graphicsQueue =  CORE::UTILS::findQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT, device);
//     if(!graphicsQueue.has_value()) {
//         throw std::runtime_error("physical device does not support VK_QUEUE_GRAPHICS_BIT");
//     }
    
//     return true;
// }
