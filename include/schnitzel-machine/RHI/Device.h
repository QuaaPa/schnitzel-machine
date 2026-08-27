#ifndef SM_RHI_DEVICE_H_
#define SM_RHI_DEVICE_H_

#include <vector>
#include <string>

#include <stdint.h>
#include <vulkan/vulkan_core.h>

#include "RHI/QueueFamilyProperties.h"
#include "core/Macros.h"
#include "RHI/QueueDescription.h"
#include "RHI/Adapter.h"
#include "RHI/QueueRequest.h"

namespace SM {    
    struct DeviceOptions {
        // Version we want the device to use, can be less than the apiVersion requested for the instance
        uint32_t apiVersion;
        std::vector<std::string> layers;
        std::vector<std::string> extensions;
        SM::AdapterFeatures requestedFeatures;        
        std::vector<QueueRequest> queues;
    };

    class Device {
    public:
        void initialize(const SM::Adapter &adapter, const SM::DeviceOptions &options, std::vector<QueueRequest> &queueRequests);
        std::vector<QueueDescription> getQueues(const std::vector<QueueRequest> &queueRequests, const std::vector<SM::QueueFamilyProperties> &queueTypes);
        
        SM_NODISCARD VkDevice getHandle() const noexcept { return m_handle; };

        void destroy();
        
    private:        
        VkDevice m_handle { VK_NULL_HANDLE };
        std::vector<QueueDescription> m_queueDescriptions;
    };
}; // namespace SM

#endif // SM_RHI_DEVICE_H_
