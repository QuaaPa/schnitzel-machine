#ifndef FRAMEBUFFER_BUILDER_HH
#define FRAMEBUFFER_BUILDER_HH

#include "core/Vulkan/VulkanFramebuffer.hh"
#include <vulkan/vulkan_core.h>

struct FramebufferBuilder {
    VkDevice logicalDevice;
    VkRenderPass renderPass;
    VkExtent2D swapchainExtent;
    std::vector<VkImageView> swapchainImageViews;
    
    VulkanFramebuffer build();
};

#endif // FRAMEBUFFER_BUILDER_HH
