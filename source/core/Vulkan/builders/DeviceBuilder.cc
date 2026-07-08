#include "core/Vulkan/builders/DeviceBuilder.h"

#include <cstdint>
#include <stdexcept>
#include <vector>
#include <set>

#include <vulkan/vulkan_core.h>

#include "utils/QueueFamilyIndices.h"

static VkPhysicalDevice pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface) {
    uint32_t physicalDeviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);
    std::vector<VkPhysicalDevice> devices(physicalDeviceCount);
    vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, devices.data());

    // TODO: add extension checker
    //    
    for (auto device : devices) {
        if (sm::findQueueFamilies(device, surface).isComplete()) {
            return device;
        }
    }
    throw std::runtime_error("no suitable GPU found");
}

DeviceBuilder::Result DeviceBuilder::build() const {
    VkPhysicalDevice physicalDevice = pickPhysicalDevice(instance, surface);
    sm::QueueFamilyIndices queueFamilyIndices = sm::findQueueFamilies(physicalDevice, surface);

    std::set<uint32_t> uniqueQueueFamilyIndices = {
        queueFamilyIndices.graphicsFamily.value(),
        queueFamilyIndices.presentFamily.value()};
    
    float priority = 1.0f;
    std::vector<VkDeviceQueueCreateInfo> deviceQueueCreateInfos;
    for (uint32_t family : uniqueQueueFamilyIndices) {
        VkDeviceQueueCreateInfo deviceQueueCreateInfo{
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = family,
            .queueCount = 1,
            .pQueuePriorities = &priority,
        };
        deviceQueueCreateInfos.push_back(deviceQueueCreateInfo);
    }
    const char *swapchainExt = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
    VkDeviceCreateInfo deviceCreateInfo{
      .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
      .queueCreateInfoCount = static_cast<uint32_t>(deviceQueueCreateInfos.size()),
      .pQueueCreateInfos = deviceQueueCreateInfos.data(),
      .enabledExtensionCount = 1,
      .ppEnabledExtensionNames = &swapchainExt
    };

    VkDevice logicalDevice;
    if (vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &logicalDevice) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device");
    }

    Result result {
        .physicalDevice = physicalDevice,
        .logicalDevice = logicalDevice,
        .graphicsFamilyIndex = queueFamilyIndices.graphicsFamily.value(),
        .presentFamilyIndex = queueFamilyIndices.presentFamily.value(),
    };

    uint32_t graphicsFamilyIndex = queueFamilyIndices.graphicsFamily.value();
    uint32_t presentFamilyIndex = queueFamilyIndices.presentFamily.value();
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    vkGetDeviceQueue(logicalDevice, graphicsFamilyIndex, 0, &graphicsQueue);
    vkGetDeviceQueue(logicalDevice, presentFamilyIndex, 0, &presentQueue);    
    
    return Result {
        .physicalDevice = physicalDevice,
        .logicalDevice = logicalDevice,
        .graphicsQueue = graphicsQueue,
        .presentQueue = presentQueue,
        .graphicsFamilyIndex = queueFamilyIndices.graphicsFamily.value(),
        .presentFamilyIndex = queueFamilyIndices.presentFamily.value(),      
    };
}
