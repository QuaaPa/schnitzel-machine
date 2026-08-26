#ifndef SM_RHI_QUEUEFAMILYPROPERTIES_H_
#define SM_RHI_QUEUEFAMILYPROPERTIES_H_

#include <vulkan/vulkan_core.h>
namespace SM {    
    struct QueueFamilyProperties {
        bool supportsFeature(VkQueueFlags featureFlags) const noexcept { return (flags & featureFlags) == featureFlags; }

        VkQueueFlags flags;
        uint32_t availableQueues;
        uint32_t timestampValidBits;
        VkExtent3D minImageTransferGranularity;
    };
    
} // namespae SM

#endif // SM_RHI_QUEUEFAMILYPROPERTIES_H_
