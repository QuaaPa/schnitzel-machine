#include "VulkanDevice.hh"
#include "VulkanInstance.hh"
#include "VulkanPhysicalDevice.hh"
#include <cstdint>
#include <vulkan/vulkan_core.h>

void CORE::Device::init(VulkanInstance &instance) {
    m_physicalDevice = new PhysicalDevice();
    VkPhysicalDevice physicalDevice = m_physicalDevice->pickPhysicalDevice(instance);

    // We may select a queue that is not supported by our device
    // 
    std::optional<uint32_t> graphicsQueueIndex = CORE::UTILS::findQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT, physicalDevice);
    
    VkDeviceQueueCreateInfo graphicsQueueCreateInfo{};    
    graphicsQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    graphicsQueueCreateInfo.queueFamilyIndex = graphicsQueueIndex.value();
    graphicsQueueCreateInfo.queueCount = 1;
    float graphicsQueuePriorities = 1.0f;
    graphicsQueueCreateInfo.pQueuePriorities = &graphicsQueuePriorities;

    VkPhysicalDeviceFeatures deviceFeatures{}; // leaving everthing to VK_FALSE

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = &graphicsQueueCreateInfo;
    createInfo.queueCreateInfoCount = 1;

    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = 0;

// #ifndef NDEBUG    
//     createInfo.enabledLayerCount = static_cast<uint32_t>(m_validationLayers.size());
//     createInfo.ppEnabledLayerNames = m_validationLayers.data();
// #else
//     createInfo.enabledLayerCount = 0;
// #endif

    createInfo.enabledLayerCount = 0;    
    
    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &m_device)  != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }

    vkGetDeviceQueue(m_device, graphicsQueueIndex.value(), 0, &m_graphicsQueue);    
}

void CORE::Device::destroy() {
    delete m_physicalDevice;
    vkDestroyDevice(m_device, nullptr);
}
