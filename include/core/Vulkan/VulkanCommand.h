#ifndef SM_CORE_VULKAN_VULKANCOMMAND_H_
#define SM_CORE_VULKAN_VULKANCOMMAND_H_

#include <vector>

#include <vulkan/vulkan_core.h>

struct VulkanCommand {
    VkCommandPool graphicsCommandPool = VK_NULL_HANDLE;
    VkCommandPool transferCommandPool = VK_NULL_HANDLE;
    std::vector<VkCommandBuffer> graphicsCommandBuffers{VK_NULL_HANDLE};
    // std::vector<VkCommandBuffer> transfercommandBuffers{VK_NULL_HANDLE};
};

#endif // SM_CORE_VULKAN_VULKANCOMMAND_H_
