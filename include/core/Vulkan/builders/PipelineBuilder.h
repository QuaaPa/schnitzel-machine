#ifndef SM_CORE_VULKAN_BUILDERS_PIPELINEBUILDER_H_
#define SM_CORE_VULKAN_BUILDERS_PIPELINEBUILDER_H_

#include <vulkan/vulkan_core.h>
#include <glm/glm.hpp>

#include "core/Vulkan/VulkanPipeline.h"

struct PipelineBuilder {
    VkDevice logicalDevice = VK_NULL_HANDLE;
    VkRenderPass renderPass = VK_NULL_HANDLE;
    VkExtent2D swapchainExtent;
    uint32_t subpass;
    
    VulkanPipeline build();
};

#endif // SM_CORE_VULKAN_BUILDERS_PIPELINEBUILDER_H_
