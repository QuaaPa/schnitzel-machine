#include "Resources/Swapchain.h"

#include "RHI/AdapterSwapchainProperties.h"
#include "RHI/VkResultToString.h"
#include "core/Log.h"
#include "core/Macros.h"
#include <cstdint>
#include <glm/fwd.hpp>
#include <vector>
#include <vulkan/vulkan_core.h>

SM::Swapchain::~Swapchain() {
    if (m_swapchain != VK_NULL_HANDLE) {
        for (auto imageView : m_imageViews) {
            vkDestroyImageView(m_device, imageView, nullptr);
        }
        vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);
        m_swapchain = VK_NULL_HANDLE;
    }
    m_device = VK_NULL_HANDLE;
}

SM::Swapchain::Swapchain(SM::Swapchain&& other) noexcept {
    m_swapchain = other.m_swapchain;
    m_images = other.m_images;
    m_imageViews = other.m_imageViews;
    m_imageFormat = other.m_imageFormat;
    m_imageExtent = other.m_imageExtent;
    m_device = other.m_device;
    other.m_swapchain = VK_NULL_HANDLE;
    other.m_device = VK_NULL_HANDLE;
}

SM::Swapchain& SM::Swapchain::operator=(SM::Swapchain&& other) noexcept {
    if (this != &other) {
        if (m_swapchain != VK_NULL_HANDLE) {
            for (auto imageView : m_imageViews) {
                vkDestroyImageView(m_device, imageView, nullptr);
            }
            vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);
            m_swapchain = VK_NULL_HANDLE;
        }
        m_swapchain = other.m_swapchain;
        m_images = other.m_images;
        m_imageViews = other.m_imageViews;
        m_imageFormat = other.m_imageFormat;
        m_imageExtent = other.m_imageExtent;
        m_device = other.m_device;
        other.m_swapchain = VK_NULL_HANDLE;
        other.m_device = VK_NULL_HANDLE;
    }
    return *this;
}

SM::Result SM::Swapchain::initializeSwapchain(VkDevice vkDevice, VkSurfaceKHR vkSurface, const SM::AdapterSwapchainProperties& supportedSwapchainProperties, const SwapchainDescription& desc) {

    m_device = vkDevice;
    m_imageFormat = desc.format;
    m_imageExtent = desc.imageExtent;    
    
    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = vkSurface;
    createInfo.minImageCount = SM_CLAMP(
                          /* desired */ desc.imageCount,
                          /* min */ supportedSwapchainProperties.capabilities.minImageCount,
                          /* max */ 16); // 16 - is hardcoded temporarily
    createInfo.imageFormat = desc.format;
    createInfo.imageColorSpace = desc.colorSpace;
    createInfo.imageExtent = desc.imageExtent;
    createInfo.imageArrayLayers = desc.imageLayers;
    createInfo.imageUsage = desc.imageUsageFlags;
    createInfo.imageSharingMode = desc.imageSharingMode;
    createInfo.queueFamilyIndexCount = desc.queueFamilyIndices.size();
    createInfo.pQueueFamilyIndices = desc.queueFamilyIndices.data();
    createInfo.preTransform = desc.transform;
    createInfo.compositeAlpha = desc.compositeAlpha;
    createInfo.presentMode = desc.presentMode;
    createInfo.clipped = desc.clipped;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    // Swapchain creation
    if(auto result = vkCreateSwapchainKHR(m_device, &createInfo, nullptr, &m_swapchain); result != VK_SUCCESS )
        return result;

    // Image objects creation
    uint32_t vkSwapchainImageCount = 0;
    vkGetSwapchainImagesKHR(m_device, m_swapchain, &vkSwapchainImageCount, nullptr);
    m_images.clear();
    m_images.resize(vkSwapchainImageCount);
    if (vkSwapchainImageCount != 0) {
        if (auto result = vkGetSwapchainImagesKHR(m_device, m_swapchain, &vkSwapchainImageCount, m_images.data()); result != VK_SUCCESS) {
            SM_LOG_ERROR("Swapchain/Image", "{}: Failed to query swapchain images", SM::toString(result));
        }
    }
    m_imageViews.clear();
    m_imageViews.resize(vkSwapchainImageCount);
    for (int i = 0; i < vkSwapchainImageCount; ++i) {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = m_images[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = desc.format;
        createInfo.components = {
            .r = VK_COMPONENT_SWIZZLE_IDENTITY,
            .g = VK_COMPONENT_SWIZZLE_IDENTITY,
            .b = VK_COMPONENT_SWIZZLE_IDENTITY,
            .a = VK_COMPONENT_SWIZZLE_IDENTITY
        };
        createInfo.subresourceRange = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1,
        };
        if (auto result = vkCreateImageView(m_device, &createInfo, nullptr, &m_imageViews[i]); result != VK_SUCCESS) {
            SM_LOG_ERROR("Swapchain/ImageView", "{}: Failed to create vkImageView[{}]", SM::toString(result), i);
        }
    }

    return VK_SUCCESS;
}
