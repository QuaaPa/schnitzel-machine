#include "core/Vulkan/VulkanPhysicalDevice.hh"
#include "core/Vulkan/VulkanInstance.hh"
#include <cstdint>
#include <string>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan_core.h>
#include <set>

VkPhysicalDevice CORE::PhysicalDevice::pickPhysicalDevice(VulkanInstance &instance) {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance.getInstance(), &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
    vkEnumeratePhysicalDevices(instance.getInstance(), &deviceCount, physicalDevices.data());
    
    for(const auto& physicalDevice : physicalDevices) {
        if (isDeviceSuitable(physicalDevice)) {
            m_physicalDevice = physicalDevice;
#ifndef NDEBUG
            VkPhysicalDeviceProperties deviceProperties;
            vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
            VkPhysicalDeviceFeatures deviceFeatures;
            vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);
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

bool CORE::PhysicalDevice::isDeviceSuitable(VkPhysicalDevice physicalDevice) {
    // TODO: check queue family support

    bool extensionsSupported = checkDeviceExtensionSupport(physicalDevice);
    
    return true;
}

bool CORE::PhysicalDevice::checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice) {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}
