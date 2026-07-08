#ifndef VULKAN_FRAMEBUFFER_HH
#define VULKAN_FRAMEBUFFER_HH

#include <vector>
#include <vulkan/vulkan_core.h>

struct VulkanFramebuffer {
    std::vector<VkFramebuffer> framebuffers{VK_NULL_HANDLE};
};

#endif // VULKAN_FRAMEBUFFER_HH
