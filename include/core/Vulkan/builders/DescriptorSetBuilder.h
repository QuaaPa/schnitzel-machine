#ifndef SM_CORE_VULKAN_BUILDERS_DESCRIPTORSETBUILDER_H_
#define SM_CORE_VULKAN_BUILDERS_DESCRIPTORSETBUILDER_H_

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "core/Vulkan/VulkanDescriptroSet.h"

struct DescriptorSetBuilder {
    VkDevice logicalDevice = VK_NULL_HANDLE;

    [[nodiscard]] VulkanDescriptorSet build() const; 
};

#endif // SM_CORE_VULKAN_BUILDERS_DESCRIPTORSETBUILDER_H_
