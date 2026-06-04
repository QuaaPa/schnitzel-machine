#include "core/Vulkan/builders/DeviceBuilder.hh"

#include "utils/QueueFamilyIndices.hh"
#include <cstdint>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan_core.h>

static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicalDevice,
                                            VkSurfaceKHR surface)
{
    QueueFamilyIndices queueFamilyIndices;
    uint32_t deviceQueueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &deviceQueueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> families(deviceQueueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &deviceQueueFamilyCount, families.data());

    for (uint32_t i = 0; i < deviceQueueFamilyCount; i++) {
        if (families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            queueFamilyIndices.graphicsFamily = i;
        }
        VkBool32 present = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &present);
        if (present) {
            queueFamilyIndices.presentFamily = i;
        }
        if (queueFamilyIndices.isComplete()) {
            break;
        }
    }
    return queueFamilyIndices;
}

static VkPhysicalDevice pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface) {
    uint32_t physicalDeviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);
    std::vector<VkPhysicalDevice> devices(physicalDeviceCount);
    vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, devices.data());

    for (auto device : devices) {
        if (findQueueFamilies(device, surface).isComplete()) {
            return device;
        }
    }
    throw std::runtime_error("no suitable GPU found");
}

DeviceBuilder::Result DeviceBuilder::build() const {
    VkPhysicalDevice physicalDevice = pickPhysicalDevice(instance, surface);
    QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice, surface);

    float priority = 1.0f;
    std::vector<VkDeviceQueueCreateInfo> deviceQueueCreateInfos;
    for (uint32_t family : {queueFamilyIndices.graphicsFamily.value(),
                            queueFamilyIndices.presentFamily.value()}) {
        VkDeviceQueueCreateInfo deviceQueueCreateInfo{
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .queueFamilyIndex = family,
            .queueCount = 1,
            .pQueuePriorities = &priority,
        };
        deviceQueueCreateInfos.push_back(deviceQueueCreateInfo);
    }

    const char *swapchainExt = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
    VkDeviceCreateInfo deviceCreateInfo{
      .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
      .queueCreateInfoCount = (uint32_t)deviceQueueCreateInfos.size(),
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
    vkGetDeviceQueue(logicalDevice, result.graphicsFamilyIndex, 0, &result.graphicsQueue);
    vkGetDeviceQueue(logicalDevice, result.presentFamilyIndex, 0,
                     &result.presentQueue);

    return result;
}
