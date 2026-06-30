#ifndef PIPELINE_BUILDER_HH
#define PIPELINE_BUILDER_HH

#include <vulkan/vulkan_core.h>
#include "core/Vulkan/VulkanPipeline.hh"

struct PipelineBuilder {
    VkDevice logicalDevice = VK_NULL_HANDLE;
    VkRenderPass renderPass = VK_NULL_HANDLE;
    VkExtent2D swapchainExtent;
    uint32_t subpass;
    
    VulkanPipeline build();
};

#endif // PIPELINE_BUILDER_HH
