#ifndef SM_RHI_ADAPTERQUEUETYPE_H_
#define SM_RHI_ADAPTERQUEUETYPE_H_

#include <vulkan/vulkan_core.h>
namespace SM {    
    struct AdapterQueueType {
        bool supportsFeature(VkQueueFlags featureFlags) const noexcept { return (flags & featureFlags) == featureFlags; }

        VkQueueFlags flags;
        uint32_t availableQueues;
        uint32_t timestampValidBits;
        VkExtent3D minImageTransferGranularity;
    };
}

#endif // SM_RHI_ADAPTERQUEUETYPE_H_
