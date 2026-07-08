#include "core/Vulkan/builders/CommandBuilder.h"

#include <cstdint>
#include <stdexcept>
#include <vector>

#include <vulkan/vulkan_core.h>

#include "core/Vulkan/VulkanCommand.h"
#include "core/Vulkan/VulkanManager.h"

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

static std::vector<VkCommandBuffer> createCommandBuffers(VkDevice logicalDevice, VkCommandPool cmdPool) {
    std::vector<VkCommandBuffer> commandBuffers(MAX_FRAMES_IN_FLIGHT);
    VkCommandBufferAllocateInfo commandBufferAllocateInfo {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = cmdPool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = (uint32_t) commandBuffers.size()
    };

    if(vkAllocateCommandBuffers(logicalDevice, &commandBufferAllocateInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }
    return commandBuffers;        
}

VulkanCommand CommandBuilder::build() {
    VkCommandPool cmdPool;
    cmdPool = createCommandPool(logicalDevice, graphicsQueueFamilyIndex);
    std::vector<VkCommandBuffer> cmdBuffers;
    cmdBuffers = createCommandBuffers(logicalDevice, cmdPool);;
    
    return VulkanCommand {
        .commandPool = cmdPool,
        .commandBuffers = cmdBuffers
    };
}
