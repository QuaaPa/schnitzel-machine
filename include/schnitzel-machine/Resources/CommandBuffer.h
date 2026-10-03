#ifndef SM_RESOURCES_COMMANDBUFFER_H_
#define SM_RESOURCES_COMMANDBUFFER_H_

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

    private:
        VkDevice m_device{ VK_NULL_HANDLE };
        VkCommandBuffer m_commandBuffer{ VK_NULL_HANDLE };
    };
}; // namespace SM

#endif // #ifndef SM_RESOURCES_COMMANDBUFFER_H_
