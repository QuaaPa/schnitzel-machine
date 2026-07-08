#include "core/Vulkan/builders/SwapchainBuilder.h"
#include "core/Vulkan/VulkanSwapchain.h"
#include "utils/QueueFamilyIndices.h"
#include "utils/SwapchainSupportDetails.h"

#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>

#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <vector>
#include <cstdint>
#include <limits>
#include <algorithm>

static sm::SwapchainSupportDetails querySwapchainSupport(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {

    sm::SwapchainSupportDetails swapchainSupportDetails;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &swapchainSupportDetails.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
    if (formatCount != 0) {
        swapchainSupportDetails.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, swapchainSupportDetails.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);
    if (presentModeCount != 0) {
        swapchainSupportDetails.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, swapchainSupportDetails.presentModes.data());
    }

    return swapchainSupportDetails;
}

static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
    for(const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
            availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }
    return availableFormats[0];
}

static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
    for(const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }        
    return VK_PRESENT_MODE_FIFO_KHR;
}

static VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities, GLFWwindow* pwindow) {
    if(capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    }
    else {
        int width, height;
        glfwGetFramebufferSize(pwindow, &width, &height);

        VkExtent2D actualExtent = {static_cast<uint32_t>(width),
                                   static_cast<uint32_t>(height)};
        
        actualExtent.width =
            std::clamp(actualExtent.width, capabilities.minImageExtent.width,
                       capabilities.maxImageExtent.width);
        actualExtent.height =
            std::clamp(actualExtent.height, capabilities.minImageExtent.height,
                       capabilities.maxImageExtent.height);

        return actualExtent;
    }
}

VulkanSwapchain SwapchainBuilder::build() const {  
    sm::SwapchainSupportDetails swapchainSupportDetails;
    swapchainSupportDetails = querySwapchainSupport(physicalDevice, surface);
    
    VkSurfaceFormatKHR swapchainImageFormat = chooseSwapSurfaceFormat(swapchainSupportDetails.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapchainSupportDetails.presentModes);
    VkExtent2D extent = chooseSwapExtent(swapchainSupportDetails.capabilities, pwindow);
    
    uint32_t imageCount = swapchainSupportDetails.capabilities.minImageCount + 1;
    if (swapchainSupportDetails.capabilities.maxImageCount > 0 && imageCount > swapchainSupportDetails.capabilities.maxImageCount) {
        imageCount = swapchainSupportDetails.capabilities.maxImageCount;
    }
    
    if(swapchainSupportDetails.formats.empty() || swapchainSupportDetails.presentModes.empty()) {
        throw std::runtime_error("current physical device does not support swapchain");
    }

    VkSwapchainCreateInfoKHR swapchainCreateInfo {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = surface,
        .minImageCount = imageCount,
        .imageFormat = swapchainImageFormat.format,
        .imageColorSpace = swapchainImageFormat.colorSpace,
        .imageExtent = extent,
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .preTransform = swapchainSupportDetails.capabilities.currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = presentMode,
        .clipped = VK_TRUE,
        .oldSwapchain = VK_NULL_HANDLE
    };

    sm::QueueFamilyIndices indices = sm::findQueueFamilies(physicalDevice, surface);
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    if (indices.graphicsFamily != indices.presentFamily) {
        swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapchainCreateInfo.queueFamilyIndexCount = 2;
        swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swapchainCreateInfo.queueFamilyIndexCount = 0; // Optional
        swapchainCreateInfo.pQueueFamilyIndices = nullptr; // Optional
    }
    VkSwapchainKHR swapchain;
    if (vkCreateSwapchainKHR(logicalDevice, &swapchainCreateInfo, nullptr, &swapchain) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain!");
    }

    std::vector<VkImage> swapchainImages;
    vkGetSwapchainImagesKHR(logicalDevice, swapchain, &imageCount, nullptr);
    swapchainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(logicalDevice, swapchain, &imageCount, swapchainImages.data());

    std::vector<VkImageView> swapchainImageViews(swapchainImages.size());
    for(size_t i = 0; i < swapchainImageViews.size(); i++) {
        VkImageViewCreateInfo createInfo {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .image = swapchainImages[i], .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = swapchainImageFormat.format,
            .components{.r = VK_COMPONENT_SWIZZLE_IDENTITY,
                        .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                        .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                        .a = VK_COMPONENT_SWIZZLE_IDENTITY
            },
            .subresourceRange {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel = 0,
                .levelCount = 1,
                .baseArrayLayer = 0,
                .layerCount = 1
            }
        };
        if(vkCreateImageView(logicalDevice, &createInfo, nullptr, &swapchainImageViews[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image views!");
        }
    }

    return VulkanSwapchain {
        .swapchain = swapchain,
        .images = swapchainImages,
        .imageViews = swapchainImageViews,
        .extent = extent,
        .format = swapchainImageFormat.format,
    };
}
