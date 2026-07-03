#include "core/Vulkan/builders/CommandBuilder.hh"
#include "core/Vulkan/VulkanCommand.hh"
#include <stdexcept>
#include <vulkan/vulkan_core.h>

static VkCommandPool createCommandPool(VkDevice logicalDevice, uint32_t graphicsQueueFamilyIndex) {
    VkCommandPool commandPool;
    VkCommandPoolCreateInfo commandPoolInfo {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = graphicsQueueFamilyIndex,
    };
    if(vkCreateCommandPool(logicalDevice, &commandPoolInfo, nullptr, &commandPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool!");
    }
    return commandPool;
}

static VkCommandBuffer createCommandBuffer(VkDevice logicalDevice, VkCommandPool cmdPool) {
    VkCommandBuffer commandBuffer;
    VkCommandBufferAllocateInfo commandBufferAllocateInfo {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = cmdPool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1
    };

    if(vkAllocateCommandBuffers(logicalDevice, &commandBufferAllocateInfo, &commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }
    return commandBuffer;        
}

// =
//
// recording command buffer
//
// =

VulkanCommand CommandBuilder::build() {
    VkCommandPool cmdPool;
    cmdPool = createCommandPool(logicalDevice, graphicsQueueFamilyIndex);
    VkCommandBuffer cmdBuffer;
    cmdBuffer = createCommandBuffer(logicalDevice, cmdPool);;
    
    return VulkanCommand {
        .commandPool = cmdPool,
        .commandBuffer = cmdBuffer
    };
}
