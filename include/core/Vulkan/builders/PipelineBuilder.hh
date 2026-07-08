#ifndef PIPELINE_BUILDER_HH
#define PIPELINE_BUILDER_HH

#include "core/Vulkan/VulkanPipeline.hh"

#include <vulkan/vulkan_core.h>

struct PipelineBuilder {
    VkDevice logicalDevice = VK_NULL_HANDLE;
    VkRenderPass renderPass = VK_NULL_HANDLE;
    VkExtent2D swapchainExtent;
    uint32_t subpass;
    
    VulkanPipeline build();
};

#endif // PIPELINE_BUILDER_HH
