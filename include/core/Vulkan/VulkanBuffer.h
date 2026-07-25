#ifndef SM_CORE_VULKAN_VULKANBUILDER_H_
#define SM_CORE_VULKAN_VULKANBUILDER_H_

#include <iostream>

#include <vulkan/vulkan_core.h>

struct VulkanBuffer {
    VkBuffer buffer = VK_NULL_HANDLE;
    VkDeviceMemory bufferMemory = VK_NULL_HANDLE;

    std::size_t size;
};

#endif // SM_CORE_VULKAN_VULKANBUILDER_H_
