#ifndef SM_CORE_VULKAN_BUILDERS_DESCRIPTORSETBUILDER_H_
#define SM_CORE_VULKAN_BUILDERS_DESCRIPTORSETBUILDER_H_

#include <cstdint>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "core/Vulkan/VulkanDescriptor.h"

struct DescriptorBuilder {
    VkDevice logicalDevice = VK_NULL_HANDLE;

    [[nodiscard]] VulkanDescriptor build() const; 
};

#endif // SM_CORE_VULKAN_BUILDERS_DESCRIPTORSETBUILDER_H_
