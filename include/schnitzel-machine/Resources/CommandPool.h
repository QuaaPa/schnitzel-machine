#ifndef SM_RESOURCES_COMMANDPOOL_H_
#define SM_RESOURCES_COMMANDPOOL_H_

#include "RHI/VkResultToString.h"
#include <cstdint>

#include <vulkan/vulkan_core.h>

namespace SM {
    class CommandPool {
    public:
        CommandPool() = default;

        CommandPool(const CommandPool&) = delete;
        CommandPool& operator=(const CommandPool&) = delete;
        CommandPool(CommandPool&& other) noexcept;
        CommandPool& operator=(CommandPool&& other) noexcept;

        SM::Result initializeCommandPool(VkDevice device, const uint32_t queueFamilyIndex);
        void destroy();
        VkCommandPool getCommandPool() const noexcept { return m_commandPool; }

    private:
        VkDevice m_device{ VK_NULL_HANDLE };
        VkCommandPool m_commandPool{ VK_NULL_HANDLE };
    };
}; // namespace SM

#endif // SM_RESOURCES_COMMANDPOOL_H_
