#ifndef RENDERPASS_BUILDER_HH
#define RENDERPASS_BUILDER_HH

#include "core/Vulkan/VulkanRenderPass.hh"
#include <vulkan/vulkan_core.h>

struct RenderPassBuilder {
    VkDevice logicalDevice = VK_NULL_HANDLE;
    VkFormat swapchainFormat;
    
    VulkanRenderPass build() const;
};

#endif // RENDERPASS_BUILDER_HH
