#include "core/Vulkan/builders/SyncObjectsBuilder.hh"pf
#include "core/Vulkan/VulkanSyncObjects.hh"

#include <stdexcept>
#include <vulkan/vulkan_core.h>

VulkanSyncObjects SyncObjectsBuilder::build() {
    if(syncObjectsSize == 0) {
        throw std::runtime_error("synchronization objects size must be greater than 0");
    }
    else {
        std::vector<VkSemaphore> imageAvailableSemaphores(syncObjectsSize);
        std::vector<VkSemaphore> renderFinishedSemaphores(syncObjectsSize);
        std::vector<VkFence> inFlightFences(MAX_FRAMES_IN_FLIGHT);

        VkSemaphoreCreateInfo semaphoreInfo {
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
        };
        VkFenceCreateInfo fenceInfo {
            .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
            .flags = VK_FENCE_CREATE_SIGNALED_BIT
        };

        for(size_t i = 0; i < syncObjectsSize; i++) {
            if (vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
                vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
                vkCreateFence(logicalDevice, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create synchronization objects for a frame!");
        
            }
        }
    
        return VulkanSyncObjects {
            .imageAvailableSemaphores = imageAvailableSemaphores,
            .renderFinishedSemaphores = renderFinishedSemaphores,
            .inFlightFences = inFlightFences
        };        
    }
}
