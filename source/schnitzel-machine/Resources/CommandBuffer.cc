#include "Resources/CommandBuffer.h"

#include <vulkan/vulkan_core.h>
#include "RHI/VkResultToString.h"
#include "core/Log.h"

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

void SM::CommandBuffer::cmdPipelineBarrier(VkPipelineStageFlags srcStageMask,
                                           VkPipelineStageFlags dstStageMask,
                                           VkDependencyFlags dependencyFlags,
                                           uint32_t memoryBarrierCount,
                                           const VkMemoryBarrier* pMemoryBarriers,
                                           uint32_t bufferMemoryBarrierCount,
                                           const VkBufferMemoryBarrier* pBufferMemoryBarriers,
                                           uint32_t imageMemoryBarrierCount,
                                           const VkImageMemoryBarrier* pImageMemoryBarriers) {
    vkCmdPipelineBarrier(m_commandBuffer,
                         srcStageMask,
                         dstStageMask,
                         dependencyFlags,
                         memoryBarrierCount, pMemoryBarriers,
                         bufferMemoryBarrierCount, pBufferMemoryBarriers,
                         imageMemoryBarrierCount, pImageMemoryBarriers);
}

void SM::CommandBuffer::cmdBeginRendering(const VkRenderingInfo* pRenderingInfo) {
    vkCmdBeginRendering(m_commandBuffer, pRenderingInfo);
}

void SM::CommandBuffer::resetCommandBuffer(VkCommandBufferResetFlags flags) {
    if (auto result = vkResetCommandBuffer(m_commandBuffer, flags); result != VK_SUCCESS) {
        SM_LOG_ERROR("CommandBuffer/Reset", "{}: Failed to reset command buffer", SM::toString(result));
    }
}

void SM::CommandBuffer::cmdBindPipeline(VkPipelineBindPoint pipelineBindPoint, VkPipeline vkPipeline) {
    vkCmdBindPipeline(m_commandBuffer, pipelineBindPoint, vkPipeline);
}

void SM::CommandBuffer::cmdSetViewport(uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pVkViewport) {
    vkCmdSetViewport(m_commandBuffer, firstViewport, viewportCount, pVkViewport);
}

void SM::CommandBuffer::cmdSetScissor(uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pVkScissors) {
    vkCmdSetScissor(m_commandBuffer, firstScissor, scissorCount, pVkScissors);
}

void SM::CommandBuffer::cmdDraw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) {
    vkCmdDraw(m_commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
}

void SM::CommandBuffer::cmdEndRendering() {
    vkCmdEndRendering(m_commandBuffer);
}

void SM::CommandBuffer::endCommandBuffer() {
    vkEndCommandBuffer(m_commandBuffer);
}

void SM::CommandBuffer::beginCommandBuffer(const VkCommandBufferBeginInfo* pVkBeginInfo) {
    vkBeginCommandBuffer(m_commandBuffer, pVkBeginInfo);
}
