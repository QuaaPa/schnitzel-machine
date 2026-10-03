#ifndef SM_RESOURCES_COMMANDBUFFER_H_
#define SM_RESOURCES_COMMANDBUFFER_H_

#include <cstdint>
#include "RHI/VkResultToString.h"
#include <vulkan/vulkan_core.h>

namespace SM {

    class CommandBuffer {
    public:
        CommandBuffer() = default;
        ~CommandBuffer();

        CommandBuffer(const CommandBuffer&) = delete;
        CommandBuffer& operator=(const CommandBuffer&) = delete;
        CommandBuffer(CommandBuffer&& other) noexcept;
        CommandBuffer& operator=(CommandBuffer&& other) noexcept;

        SM::Result initializeCommandBuffer(VkDevice vkDevice, VkCommandPool vkCommandPool);
        VkCommandBuffer getCommandBuffer() const noexcept { return m_commandBuffer; }

        void cmdPipelineBarrier(VkPipelineStageFlags srcStageMask,
                                VkPipelineStageFlags dstStageMask,
                                VkDependencyFlags dependencyFlags,
                                uint32_t memoryBarrierCount,
                                const VkMemoryBarrier* pMemoryBarriers,
                                uint32_t bufferMemoryBarrierCount,
                                const VkBufferMemoryBarrier* pBufferMemoryBarriers,
                                uint32_t imageMemoryBarrierCount,
                                const VkImageMemoryBarrier* pImageMemoryBarriers);
        void cmdBeginRendering(const VkRenderingInfo* pRenderingInfo);
        void cmdBindPipeline(VkPipelineBindPoint pipelineBindPoint, VkPipeline vkPipeline);
        void resetCommandBuffer(VkCommandBufferResetFlags flags);
        void cmdSetViewport(uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pVkViewport);
        void cmdSetScissor(uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pVkScissors);
        void cmdDraw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);
        void cmdEndRendering();
        void endCommandBuffer();

    private:
        VkDevice m_device{ VK_NULL_HANDLE };
        VkCommandBuffer m_commandBuffer{ VK_NULL_HANDLE };
    };
}; // namespace SM

#endif // #ifndef SM_RESOURCES_COMMANDBUFFER_H_
