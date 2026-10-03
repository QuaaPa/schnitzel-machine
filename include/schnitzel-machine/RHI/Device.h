#ifndef SM_RHI_DEVICE_H_
#define SM_RHI_DEVICE_H_

#include <stdint.h>

#include <cstdint>
#include <string>
#include <vector>

#include <vulkan/vulkan_core.h>

#include "RHI/Adapter.h"
#include "RHI/Queue.h"
#include "RHI/QueueRequest.h"
#include "core/Macros.h"

namespace SM {
    struct DeviceOptions {
        std::vector<std::string> layers;
        std::vector<std::string> extensions;
        SM::AdapterFeatures requestedFeatures;
        std::vector<QueueRequest> queues;
    };

    class Device {
    public:
        Device(uint32_t apiVersion, const SM::Adapter* pAdapter, const SM::DeviceOptions& options);
        ~Device();

        std::vector<Queue> getQueues() const noexcept { return m_queues; }
        void waitIdle();

        SM_NODISCARD VkDevice getHandle() const noexcept { return m_handle; };
        void destroy();

    private:
        VkDevice m_handle{ VK_NULL_HANDLE };
        std::vector<Queue> m_queues;
    };
}; // namespace SM

#endif // SM_RHI_DEVICE_H_
