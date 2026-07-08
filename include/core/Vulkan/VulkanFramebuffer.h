#ifndef SM_CORE_VULKAN_VULKANFRAMEBUFFER_H_
#define SM_CORE_VULKAN_VULKANFRAMEBUFFER_H_

#include <vector>
#include <vulkan/vulkan_core.h>

struct VulkanFramebuffer {
    std::vector<VkFramebuffer> framebuffers{VK_NULL_HANDLE};
};

#endif // SM_CORE_VULKAN_VULKANFRAMEBUFFER_H_
