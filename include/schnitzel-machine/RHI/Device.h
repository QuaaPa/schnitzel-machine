#ifndef SM_RHI_DEVICE_H_
#define SM_RHI_DEVICE_H_

#include <vector>
#include <string>
#include <span>

#include <stdint.h>
#include <vulkan/vulkan_core.h>

#include "core/Macros.h"
#include "core/TypesDefs.h"
#include "RHI/Adapter.h"
#include "RHI/AdapterQueueType.h"

namespace SM {    
    struct QueueRequest {
        uint32_t familyIndex;
        VkQueueFlags flags;
        uint32_t count;
        std::vector<float> priorities;
    };

    struct DeviceOptions {
        // Version we want the device to use, can be less than the apiVersion requested for the instance
        uint32_t apiVersion{ SM_MAKE_VERSION(1, 2, 0) };
        std::vector<std::string> layers;
        std::vector<std::string> extensions;
        SM::AdapterFeatures requestedFeatures;        
        std::vector<QueueRequest> queues;
    };

    struct Device {
    public:
        Device();
        ~Device();        
        
    public:
        SM::SMResult initialize(SM::Adapter adapter, const SM::DeviceOptions& options, std::vector<QueueRequest> &queueRequests);
        std::vector<QueueDescription> getQueues(const std::vector<QueueRequest> &queueRequests, std::vector<SM::AdapterQueueType> queueTypes);
        
        SM_NODISCARD VkDevice getHandle() { return m_handle; };

        SM::SMResult destroy();
        
    private:        
        VkDevice m_handle { VK_NULL_HANDLE };
        std::vector<QueueDescription> m_queueDescriptions;
    };
}; // namespace SM

#endif // SM_RHI_DEVICE_H_
