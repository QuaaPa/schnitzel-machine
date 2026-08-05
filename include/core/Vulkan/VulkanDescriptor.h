#ifndef SM_CORE_VULKAN_DESCRIPTORSET_H_
#define SM_CORE_VULKAN_DESCRIPTORSET_H_

#include <vector>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

struct VulkanDescriptor {
    VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
    VkDescriptorPool descriptorPool = VK_NULL_HANDLE;
    std::vector<VkDescriptorSet> descriptorSets{VK_NULL_HANDLE};
};

#endif // SM_CORE_VULKAN_DESCRIPTORSET_H_
