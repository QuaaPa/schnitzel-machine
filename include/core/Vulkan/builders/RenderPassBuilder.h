#ifndef SM_CORE_VULKAN_BUILDERS_RENDERPASSBUILDER_H_
#define SM_CORE_VULKAN_BUILDERS_RENDERPASSBUILDER_H_

#include <vulkan/vulkan_core.h>

#include "core/Vulkan/VulkanRenderPass.h"

struct RenderPassBuilder {
    VkDevice logicalDevice = VK_NULL_HANDLE;
    VkFormat swapchainFormat;
    
    VulkanRenderPass build() const;
};

#endif // SM_CORE_VULKAN_BUILDERS_RENDERPASSBUILDER_H_
