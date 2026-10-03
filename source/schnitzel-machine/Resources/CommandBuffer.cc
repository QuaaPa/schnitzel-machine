#include "Resources/CommandBuffer.h"

#include <vulkan/vulkan_core.h>

SM::CommandBuffer::~CommandBuffer() {
    m_commandBuffer = VK_NULL_HANDLE;
    m_device = VK_NULL_HANDLE;
}

SM::CommandBuffer::CommandBuffer(SM::CommandBuffer&& other) noexcept {
    m_commandBuffer = other.m_commandBuffer;
    m_device = other.m_device;
    other.m_commandBuffer = VK_NULL_HANDLE;
    other.m_device = VK_NULL_HANDLE;
}

SM::CommandBuffer& SM::CommandBuffer::operator=(SM::CommandBuffer&& other) noexcept {
    if (this != &other) {
        m_commandBuffer = other.m_commandBuffer;
        m_device = other.m_device;
        other.m_commandBuffer = VK_NULL_HANDLE;
        other.m_device = VK_NULL_HANDLE;
    }
    return *this;
}

SM::Result SM::CommandBuffer::initializeCommandBuffer(VkDevice vkDevice, VkCommandPool vkCommandPool) {
    m_device = vkDevice;
    
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = vkCommandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    return vkAllocateCommandBuffers(m_device, &allocInfo, &m_commandBuffer);    
}
