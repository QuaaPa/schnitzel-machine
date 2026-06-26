#ifndef VULKAN_PIPELINE_HH
#define VULKAN_PIPELINE_HH

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

struct VulkanPipeline {
    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
};

#endif //  VULKAN_PIPELINE_HH
