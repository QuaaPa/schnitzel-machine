#ifndef SM_CORE_VULKAN_BUILDERS_BUFFERBUILDER_H_
#define SM_CORE_VULKAN_BUILDERS_BUFFERBUILDER_H_

#include <cstddef>
#include <vulkan/vulkan_core.h>

#include "core/Vulkan/VulkanBuffer.h"

struct BufferBuilder {
    VkDevice logicalDevice = VK_NULL_HANDLE;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkCommandPool cmdPool = VK_NULL_HANDLE;
    VkQueue queue = VK_NULL_HANDLE;

    VkBufferUsageFlags usage;
    
    const void* srcData;
    VkDeviceSize bufferSize;
    std::size_t count;
    
    VulkanBuffer build();
};

#endif // SM_CORE_VULKAN_BUILDERS_BUFFERBUILDER_H_
