#ifndef SM_CORE_VULKAN_VULKANPIPELINE_H_
#define SM_CORE_VULKAN_VULKANPIPELINE_H_

#include <vulkan/vulkan_core.h>

struct VulkanPipeline {
    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
    VkRenderPass renderPass = VK_NULL_HANDLE;
    VkPipeline pipeline = VK_NULL_HANDLE;

    VkBuffer vertexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory vertexBufferMemory = VK_NULL_HANDLE;
};

#endif //  SM_CORE_VULKAN_VULKANPIPELINE_H_
