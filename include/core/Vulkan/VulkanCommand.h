#ifndef SM_CORE_VULKAN_VULKANCOMMAND_H_
#define SM_CORE_VULKAN_VULKANCOMMAND_H_

#include <vulkan/vulkan_core.h>
#include <vector>

struct VulkanCommand {
    VkCommandPool commandPool = VK_NULL_HANDLE;
    std::vector<VkCommandBuffer> commandBuffers{VK_NULL_HANDLE};
};

#endif // SM_CORE_VULKAN_VULKANCOMMAND_H_
