#include "RHI/Swapchain.h"

#include "RHI/VkResultToString.h"
#include <vulkan/vulkan_core.h>
#include "RHI/Adapter.h"
#include "core/Log.h"

void SM::Swapchain::initialize(const SM::Adapter &adapter, const VkDevice &deviceHandle, const VkSurfaceKHR &surfaceHandle, const SM::SwapchainOptions &options) {
    
    SM::AdapterSwapchainProperties properties = adapter.querySwapchainProperties(surfaceHandle);          

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surfaceHandle;
    createInfo.minImageCount = options.minImageCount;
    createInfo.imageFormat = options.format;
    createInfo.imageColorSpace = options.colorSpace;
    createInfo.imageExtent = {
        .width = options.imageExtent.width,
        .height = options.imageExtent.height
    };
    createInfo.imageArrayLayers = options.imageLayers;
    createInfo.imageUsage = options.imageUsageFlags;
    createInfo.imageSharingMode = options.imageSharingMode;
    if (!options.queueTypeIndices.empty()) {
        createInfo.queueFamilyIndexCount = options.queueTypeIndices.size();
        createInfo.pQueueFamilyIndices = options.queueTypeIndices.data();
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
