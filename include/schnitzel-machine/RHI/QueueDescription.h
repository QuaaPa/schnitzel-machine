#ifndef SM_RHI_QUEUEDESCRIPTOR_H_
#define SM_RHI_QUEUEDESCRIPTOR_H_

#include <vulkan/vulkan_core.h>

namespace SM {
    struct QueueDescription {
        const VkQueue queue;
        VkQueueFlags flags;
        uint32_t timestampValidBits;
        VkExtent3D minImageTransferGranularity;
        uint32_t familyIndex;
    };    
}; // namespae SM

#endif // SM_RHI_QUEUEDESCRIPTOR_H_
