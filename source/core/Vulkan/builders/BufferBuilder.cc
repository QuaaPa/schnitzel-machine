#include "core/Vulkan/builders/BufferBuilder.h"

#include <stdexcept>
#include <cstring>

#include <vulkan/vulkan_core.h>

#include "core/Vulkan/VulkanBuffer.h"

static uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice physicalDevice) {
    VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &physicalDeviceMemoryProperties);
    for (uint32_t i = 0; i < physicalDeviceMemoryProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }
    throw std::runtime_error("failed to find suitable memory type!");
}

static void createBuffer(VkDevice logicalDevice, VkPhysicalDevice physicalDevice,
                         VkDeviceSize size,
                         VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
                         VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
    VkBufferCreateInfo bufferInfo {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = size,
        .usage = usage,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE  
    };
    if(vkCreateBuffer(logicalDevice, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create vertex buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(logicalDevice, buffer, &memRequirements);

    VkMemoryAllocateInfo memoryAllocateInfo {
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .allocationSize = memRequirements.size,
        .memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits,
                                          properties,
                                          physicalDevice
                                          )
    };

    if(vkAllocateMemory(logicalDevice, &memoryAllocateInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate vertex buffer memory!");
    }

    vkBindBufferMemory(logicalDevice, buffer, bufferMemory, 0);

}

static void copyBuffer(VkDevice logicalDevice,
                       VkBuffer srcBuffer, VkBuffer dstBuffer,
                       VkDeviceSize size,
                       VkCommandPool cmdPool, VkQueue queue) {
    VkCommandBufferAllocateInfo allocInfo {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = cmdPool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1,    
    };

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(logicalDevice, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
    };

    vkBeginCommandBuffer(commandBuffer, &beginInfo);
    VkBufferCopy copyRegion {
        .srcOffset = 0,
        .dstOffset = 0,
        .size = size
    };
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
    vkEndCommandBuffer(commandBuffer);
    VkSubmitInfo submitInfo {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .commandBufferCount = 1,
        .pCommandBuffers = &commandBuffer
    };

    vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(queue);
    vkFreeCommandBuffers(logicalDevice, cmdPool, 1, &commandBuffer);
}

VulkanBuffer BufferBuilder::build() {
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(logicalDevice, physicalDevice,
                 bufferSize,
                 VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                 stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, srcData, bufferSize);
    vkUnmapMemory(logicalDevice, stagingBufferMemory);

    VkBuffer buffer;
    VkDeviceMemory bufferMemory;
    createBuffer(logicalDevice, physicalDevice,
                 bufferSize,
                 VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage,
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                 buffer, bufferMemory);

    copyBuffer(logicalDevice, stagingBuffer, buffer, bufferSize, cmdPool, queue);

    vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
    vkFreeMemory(logicalDevice, stagingBufferMemory, nullptr);  
    
    return VulkanBuffer {
        .buffer = buffer,
        .bufferMemory = bufferMemory,
        .size = count
    };
}
