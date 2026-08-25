#include "RHI/Swapchain.h"

#include "RHI/Adapter.h"
#include "core/TypesDefs.h"
#include "core/Log.h"
#include <vulkan/vulkan_core.h>

SM::Swapchain::~Swapchain() {
    if (m_handle != VK_NULL_HANDLE) {
        SM_LOG_CRITICAL("RHI/Instance", "Instance was not explicitly destroyed, forcing cleanup");
        destroy();
    }
}

SM::SMResult SM::Swapchain::initialize(SM::Adapter adapter,  VkDevice device, const SM::SwapchainOptions &options) {
    m_deviceHandle = device;
    SM::AdapterSwapchainProperties properties = adapter.querySwapchainProperties(options.surface);           
    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = options.surface;
    createInfo.minImageCount = options.minImageCount;
    createInfo.imageFormat = options.format;
    createInfo.imageColorSpace = options.colorSpace;
    createInfo.imageExtent = { .width = options.imageExtent.width, .height = options.imageExtent.height };
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
    
    if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &m_handle) != VK_SUCCESS) {
        SM_LOG_ERROR("RHI/Swapchain", "Failed to create swapchain");
        return {};
    }
    
    return SM_SUCCESS;
}

SM::SMResult SM::Swapchain::destroy() {
    if(m_handle !=  VK_NULL_HANDLE) {
        vkDestroySwapchainKHR(m_deviceHandle, m_handle, nullptr);        
    }
    m_handle = VK_NULL_HANDLE;
    
    return SM_SUCCESS;
}
