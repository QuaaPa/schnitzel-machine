#ifndef SM_RHI_QUEUEREQUEST_H_
#define SM_RHI_QUEUEREQUEST_H_

#include <vulkan/vulkan_core.h>
#include <cstdint>
#include <vector>

namespace SM {    
    struct QueueRequest {
        uint32_t familyIndex;
        VkQueueFlags flags;
        uint32_t count;
        std::vector<float> priorities;
    };
} // namespace SM

#endif // SM_RHI_QUEUEREQUEST_H_
