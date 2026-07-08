#ifndef FRAMEBUFFER_BUILDER_HH
#define FRAMEBUFFER_BUILDER_HH

#include "core/Vulkan/VulkanFramebuffer.hh"
#include <vulkan/vulkan_core.h>

struct FramebufferBuilder {
    VkDevice logicalDevice = VK_NULL_HANDLE;
    VkRenderPass renderPass = VK_NULL_HANDLE;
    VkExtent2D swapchainExtent;
    std::vector<VkImageView> swapchainImageViews{VK_NULL_HANDLE};
    
    VulkanFramebuffer build() const;
};

#endif // FRAMEBUFFER_BUILDER_HH
