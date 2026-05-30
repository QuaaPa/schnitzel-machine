#include "core/Vulkan/VulkanDevice.hh"
#include "core/Vulkan/VulkanInstance.hh"
#include "core/Vulkan/VulkanPhysicalDevice.hh"
#include "core/Vulkan/VulkanWindowSurface.hh"
#include <cstdint>
#include <vulkan/vulkan_core.h>

void CORE::Device::init(VulkanInstance &instance, WindowSurface surface) {
    m_physicalDevice = new PhysicalDevice();
    VkPhysicalDevice physicalDevice = m_physicalDevice->pickPhysicalDevice(instance);
    
    QueueFamilyIndices queueFamilyIndices{};
    findQueueFamilies(physicalDevice, surface.getSurface(), &queueFamilyIndices);
    // Graphics family
    // Present family

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {queueFamilyIndices.graphicsFamily,
                                              queueFamilyIndices.presentFamily};
    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }
    
    VkPhysicalDeviceFeatures deviceFeatures{}; // leaving everthing to VK_FALSE
    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = 0;
    createInfo.enabledLayerCount = 0;    
    
    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &m_device)  != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }
    
    vkGetDeviceQueue(m_device, queueFamilyIndices.graphicsFamily, 0, &m_graphicsQueue);
    vkGetDeviceQueue(m_device, queueFamilyIndices.presentFamily, 0, &m_presentQueue);    
}

void CORE::Device::findQueueFamilies(VkPhysicalDevice physicalDevice,
                                     VkSurfaceKHR surface,
                                     QueueFamilyIndices *pqueueFamilyIndices) {  
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());
    int i = 0;
    for (const auto &queueFamily : queueFamilies) {
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);
        if(presentSupport) {
            pqueueFamilyIndices->presentFamily = i;
        }
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {             
            pqueueFamilyIndices->graphicsFamily = i;
        }
    }
    
}    

void CORE::Device::destroy() {
    delete m_physicalDevice;
    vkDestroyDevice(m_device, nullptr);
}
