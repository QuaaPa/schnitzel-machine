#ifndef SM_RHI_QUEUE_H_
#define SM_RHI_QUEUE_H_

#include <vulkan/vulkan_core.h>

#include "core/TypesDefs.h"

namespace SM {    
    struct Queue {
    public:
        Queue(const VkDevice &device, const SM::QueueDescription &queueDescription);
        ~Queue();

        VkFlags flags() const noexcept { return m_flags; }
        uint32_t timestampValidBits() const noexcept { return m_timestampValidBits; }
        VkExtent3D minImageTransferGranularity() const noexcept { return m_minImageTransferGranularity; }
        uint32_t queueFamilyIndex() const noexcept { return m_queueFamilyIndex; }

    private:
        VkDevice m_device;
        VkQueue m_queue;
        VkFlags m_flags;
        uint32_t m_timestampValidBits;
        VkExtent3D m_minImageTransferGranularity;
        uint32_t m_queueFamilyIndex;
    };
};

#endif // SM_RHI_QUEUE_H_
