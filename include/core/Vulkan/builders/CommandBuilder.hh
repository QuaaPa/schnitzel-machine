#ifndef COMMAND_BUILDER_HH
#define COMMAND_BUILDER_HH

#include "core/Vulkan/VulkanCommand.hh"

#include <vulkan/vulkan_core.h>

struct CommandBuilder {
    VkDevice logicalDevice = VK_NULL_HANDLE;
    uint32_t graphicsQueueFamilyIndex;
    uint32_t bufferCount;

    VulkanCommand build();
};

#endif // COMMAND_BUILDER_HH
