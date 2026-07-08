#ifndef SM_CORE_VULKAN_VULKANRENDERPASS_H_
#define SM_CORE_VULKAN_VULKANRENDERPASS_H_

#include <cstdint>

#include <vulkan/vulkan_core.h>

struct VulkanRenderPass {
    VkRenderPass renderPass = VK_NULL_HANDLE;
    uint32_t subpass;
};

#endif // SM_CORE_VULKAN_VULKANRENDERPASS_H_
