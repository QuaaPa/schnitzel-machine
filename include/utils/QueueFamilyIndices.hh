#ifndef QUEUE_FAMILY_INDICIES_HH
#define QUEUE_FAMILY_INDICIES_HH

#include <optional>
#include <cstdint>
#include <vector>
#include <vulkan/vulkan.h>

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() const {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
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


#endif // QUEUE_FAMILY_INDICIES_HH
