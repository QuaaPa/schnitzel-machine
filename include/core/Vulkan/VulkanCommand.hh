#ifndef VULKAN_COMMAND_HH
#define VULKAN_COMMAND_HH

#include <vulkan/vulkan_core.h>

struct VulkanCommand {
    VkCommandPool commandPool = VK_NULL_HANDLE;
    VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
};

#endif // VULKAN_COMMAND_HH
