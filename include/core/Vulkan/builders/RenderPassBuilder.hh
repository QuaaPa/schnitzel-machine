#ifndef RENDERPASS_BUILDER_HH
#define RENDERPASS_BUILDER_HH

#include "core/Vulkan/VulkanRenderPass.hh"

struct RenderPassBuilder {
    VkDevice logicalDevice = VK_NULL_HANDLE;
    VkFormat swapchainFormat;
    
    VulkanRenderPass build();
};

#endif // RENDERPASS_BUILDER_HH
