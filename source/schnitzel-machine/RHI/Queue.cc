#include "RHI/Queue.h"

SM::Queue::Queue(const SM::QueueDescription &description)
    : m_queue(description.queue)
    , m_flags(description.flags)
    , m_timestampValidBits(description.timestampValidBits)
    , m_minImageTransferGranularity(description.minImageTransferGranularity)
    , m_queueFamilyIndex(description.familyIndex)
{
    
}

SM::Queue::~Queue()
{
    
}
