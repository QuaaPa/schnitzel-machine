#ifndef SM_RHI_QUEUEFAMILYPROPERTIES_H_
#define SM_RHI_QUEUEFAMILYPROPERTIES_H_

#include <cstdint>
#include <vulkan/vulkan_core.h>
namespace SM {
    // QueueFamilyProperties represents one single queue family
    struct QueueFamilyProperties {
        bool supportsFeature(VkQueueFlags featureFlags) const noexcept { return (flags & featureFlags) == featureFlags; }
        
        uint32_t index; // QueueFamilyIndex
        VkQueueFlags flags; // Flags supported by this family
        uint32_t availableQueues; 
        uint32_t timestampValidBits;
        VkExtent3D minImageTransferGranularity;
    };
    
} // namespae SM

#endif // SM_RHI_QUEUEFAMILYPROPERTIES_H_
