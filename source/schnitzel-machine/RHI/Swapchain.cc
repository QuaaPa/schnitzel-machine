#include "RHI/Swapchain.h"

#include <cstdint>
#include <vector>
#include <vulkan/vulkan_core.h>

#include "RHI/VkResultToString.h"
#include "RHI/VulkanConfig.h"
#include "RHI/Adapter.h"
#include "core/Macros.h"
#include "core/Log.h"

void SM::Swapchain::initialize(const SM::Adapter &adapter, const VkDevice &deviceHandle, const std::vector<SM::Queue> &queues, const VkSurfaceKHR &surfaceHandle, const SM::SwapchainOptions &options) {
    
    SM::AdapterSwapchainProperties properties = adapter.querySwapchainProperties(surfaceHandle);  

    // A value of 0 of maxImageCount means that there is no limit on the number of images,
    // but in this case we have predefined SM_MAX_IMAGE_COUNT
    uint32_t maxImageCount = properties.capabilities.maxImageCount;
    if(maxImageCount == 0) maxImageCount = SM_MAX_IMAGE_COUNT;
    
    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surfaceHandle;
    createInfo.minImageCount = SM_CLAMP(options.imageCount,
                                        properties.capabilities.minImageCount,
                                        maxImageCount);
    createInfo.imageFormat = options.format;
    createInfo.imageColorSpace = options.colorSpace;
    createInfo.imageExtent = {
        .width = options.imageExtent.width,
        .height = options.imageExtent.height
    };
    createInfo.imageArrayLayers = options.imageLayers;
    createInfo.imageUsage = options.imageUsageFlags;
    createInfo.imageSharingMode = options.imageSharingMode;

    std::vector<uint32_t> queueFamilyIndices;
    queueFamilyIndices.reserve(queues.size());
    for(const auto& queue : queues) {
        queueFamilyIndices.push_back(queue.queueFamilyIndex());
    }
    if (!queueFamilyIndices.empty()) {
        createInfo.queueFamilyIndexCount = queueFamilyIndices.size();
        createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
    }
    createInfo.preTransform = options.transform;
    createInfo.compositeAlpha = options.compositeAlpha;
    createInfo.presentMode = options.presentMode;
    createInfo.clipped = options.clipped;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (auto result = vkCreateSwapchainKHR(deviceHandle, &createInfo, nullptr, &m_handle); result != VK_SUCCESS) {
        SM_LOG_ERROR("RHI/Swapchain", "{}: Failed to create swapchain", SM::toString(result));     
    }    
}

void SM::Swapchain::destroy(const VkDevice &deviceHandle) {
    if(m_handle !=  VK_NULL_HANDLE) {
        vkDestroySwapchainKHR(deviceHandle, m_handle, nullptr);        
        m_handle = VK_NULL_HANDLE;
    }    
}
