#ifndef SM_RHI_QUEUE_H_
#define SM_RHI_QUEUE_H_

#include <vulkan/vulkan_core.h>

namespace SM {
    struct Queue {
        VkQueue      queue{ VK_NULL_HANDLE };
        VkQueueFlags flags{ 0 };
        uint32_t     timestampValidBits;
        VkExtent3D   minImageTransferGranularity;
        uint32_t     familyIndex;        
    };
}; // namespace SM

#endif // SM_RHI_QUEUE_H_
