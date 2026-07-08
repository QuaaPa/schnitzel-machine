#ifndef SM_CORE_VULKAN_BUILDERS_COMMANDBUILDER_H_
#define SM_CORE_VULKAN_BUILDERS_COMMANDBUILDER_H_

#include "core/Vulkan/VulkanCommand.h"

#include <vulkan/vulkan_core.h>

struct CommandBuilder {
    VkDevice logicalDevice = VK_NULL_HANDLE;
    uint32_t graphicsQueueFamilyIndex;
    uint32_t bufferCount;

    VulkanCommand build();
};

#endif // SM_CORE_VULKAN_BUILDERS_COMMANDBUILDER_H_
