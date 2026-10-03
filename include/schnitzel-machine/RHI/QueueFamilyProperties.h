#ifndef SM_RHI_QUEUEFAMILYPROPERTIES_H_
#define SM_RHI_QUEUEFAMILYPROPERTIES_H_

#include <cstdint>

#include <vulkan/vulkan_core.h>

#include "RHI/VkResultToString.h"
#include "core/Log.h"

namespace SM {
    // QueueFamilyProperties represents one single queue family
    struct QueueFamilyProperties {
        bool supportsFeature(VkQueueFlags featureFlags) const noexcept { return (flags & featureFlags) == featureFlags; }
        bool supportsPresentation(VkPhysicalDevice vkPhysicalDevice, VkSurfaceKHR vkSurface) const {
            VkBool32 canPresent = false;
            if (auto result = vkGetPhysicalDeviceSurfaceSupportKHR(vkPhysicalDevice, index, vkSurface, &canPresent); result != VK_SUCCESS) {
                SM_LOG_ERROR("RHI/Adapter", "{}: Failed to check presentation support for queue family index {}", SM::toString(result), index);
            }
            return canPresent;
        }

        uint32_t index;     // QueueFamilyIndex
        VkQueueFlags flags; // Flags supported by this family
        uint32_t availableQueueCount;
        uint32_t timestampValidBits;
        VkExtent3D minImageTransferGranularity;
    };

} // namespace SM

#endif // SM_RHI_QUEUEFAMILYPROPERTIES_H_
