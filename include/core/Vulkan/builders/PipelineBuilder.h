#ifndef SM_CORE_VULKAN_BUILDERS_PIPELINEBUILDER_H_
#define SM_CORE_VULKAN_BUILDERS_PIPELINEBUILDER_H_

#include "core/Vulkan/VulkanPipeline.h"

#include <vulkan/vulkan_core.h>

struct PipelineBuilder {
    VkDevice logicalDevice = VK_NULL_HANDLE;
    VkRenderPass renderPass = VK_NULL_HANDLE;
    VkExtent2D swapchainExtent;
    uint32_t subpass;
    
    VulkanPipeline build();
};

#endif // SM_CORE_VULKAN_BUILDERS_PIPELINEBUILDER_H_
