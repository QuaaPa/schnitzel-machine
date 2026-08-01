#ifndef SM_CORE_VULKAN_BUILDERS_PIPELINEBUILDER_H_
#define SM_CORE_VULKAN_BUILDERS_PIPELINEBUILDER_H_

#include <vulkan/vulkan_core.h>

#include "core/Vulkan/VulkanPipeline.h"

struct PipelineBuilder {
    VkDevice logicalDevice = VK_NULL_HANDLE;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkRenderPass renderPass = VK_NULL_HANDLE;
    VkExtent2D swapchainExtent;
    uint32_t subpass;
    
    [[nodiscard]] VulkanPipeline build();
};

#endif // SM_CORE_VULKAN_BUILDERS_PIPELINEBUILDER_H_
