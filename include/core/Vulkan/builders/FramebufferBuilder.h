#ifndef SM_CORE_VULKAN_BUILDERS_FRAMEBUFFERBUILDER_H_
#define SM_CORE_VULKAN_BUILDERS_FRAMEBUFFERBUILDER_H_

#include "core/Vulkan/VulkanFramebuffer.h"
#include <vulkan/vulkan_core.h>

struct FramebufferBuilder {
    VkDevice logicalDevice = VK_NULL_HANDLE;
    VkRenderPass renderPass = VK_NULL_HANDLE;
    VkExtent2D swapchainExtent;
    std::vector<VkImageView> swapchainImageViews{VK_NULL_HANDLE};
    
    VulkanFramebuffer build() const;
};

#endif // SM_CORE_VULKAN_BUILDERS_FRAMEBUFFERBUILDER_H_
