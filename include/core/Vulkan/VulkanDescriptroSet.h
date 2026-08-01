#ifndef SM_CORE_VULKAN_DESCRIPTORSET_H_
#define SM_CORE_VULKAN_DESCRIPTORSET_H_

#include <vulkan/vulkan.h>

struct VulkanDescriptorSet {
    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout;
};

#endif // SM_CORE_VULKAN_DESCRIPTORSET_H_
