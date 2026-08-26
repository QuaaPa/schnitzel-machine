#include "RHI/Queue.h"

#include "core/TypesDefs.h"

SM::Queue::Queue(const VkDevice &device, const SM::QueueDescription &description)
    : m_device(&device)
    , m_queue(description.queue)
    , m_flags(description.flags)
    , m_timestampValidBits(description.timestampValidBits)
    , m_minImageTransferGranularity(description.minImageTransferGranularity)
    , m_queueFamilyIndex(description.familyIndex)
{
    
}

SM::Queue::~Queue()
{
    
}
