#ifndef VULKAN_RENDERPASS_HH
#define VULKAN_RENDERPASS_HH

#include <cstdint>
#include <vulkan/vulkan_core.h>

struct VulkanRenderPass {
    VkRenderPass renderPass = VK_NULL_HANDLE;
    uint32_t subpass;
};

#endif // VULKAN_RENDERPASS_HH
