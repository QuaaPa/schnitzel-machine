#ifndef SM_UTILS_SYNCOBJECTSUTILS_H_
#define SM_UTILS_SYNCOBJECTSUTILS_H_

#include <stdexcept>

#include <vulkan/vulkan_core.h>

namespace sm {
    static VkFence createFence(VkDevice logicalDevice, VkFenceCreateFlags flags = 0) {
        VkFence fence;
        VkFenceCreateInfo fenceInfo {
            .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
            .pNext = nullptr,
            .flags = flags
        };
        if(vkCreateFence(logicalDevice, &fenceInfo, nullptr, &fence) != VK_SUCCESS) {
            throw std::runtime_error("failed to create fence!");
        }
        return fence; 
    }

    static VkSemaphore createSemaphore(VkDevice logicalDevice) {
        VkSemaphore semaphore = VK_NULL_HANDLE;
        VkSemaphoreCreateInfo semaphoreInfo {
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0 // must be 0
        };

        if(vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &semaphore) != VK_SUCCESS) {
            throw std::runtime_error("failed to create semaphore!");
        }
    
        return semaphore;
    }
          
}

#endif // SM_UTILS_SYNCOBJECTSUTILS_H_
