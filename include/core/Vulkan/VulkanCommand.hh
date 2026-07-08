#ifndef VULKAN_COMMAND_HH
#define VULKAN_COMMAND_HH

#include <vulkan/vulkan_core.h>
#include <vector>


struct VulkanCommand {
    VkCommandPool commandPool = VK_NULL_HANDLE;
    std::vector<VkCommandBuffer> commandBuffers{VK_NULL_HANDLE};
};

#endif // VULKAN_COMMAND_HH
