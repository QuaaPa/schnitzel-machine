#ifndef SM_CORE_VULKAN_BUILDERS_COMMANDBUILDER_H_
#define SM_CORE_VULKAN_BUILDERS_COMMANDBUILDER_H_

#include <vector>
#include <cstdint>

#include <vulkan/vulkan_core.h>

#include "core/Vulkan/VulkanCommand.h"

struct CommandBuilder {
    VkDevice logicalDevice = VK_NULL_HANDLE;
    uint32_t graphicsQueueFamilyIndex;
    // uint32_t transferQueueFamilyIndex;

    [[nodiscard]] VulkanCommand build();
};

#endif // SM_CORE_VULKAN_BUILDERS_COMMANDBUILDER_H_
