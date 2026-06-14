#include "core/Vulkan/builders/SwapchainBuilder.hh"
#include "core/Vulkan/SwapchainBuilder.hh"
#include "core/Vulkan/VulkanSwapchain.hh"
#include "utils/QueueFamilyIndices.hh"
#include "utils/SwapchainSupportDetails.hh"
#include <GLFW/glfw3.h>
#include <cstdint>
#include <stdexcept>
#include <vector>
#include <cstdint>
#include <limits>
#include <algorithm>
#include <vulkan/vulkan_core.h>

static SwapchainSupportDetails querySwapchainSupport(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
    SwapchainSupportDetails swapchainSupportDetails;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &swapchainSupportDetails.surfaceCapabilities);

    // uint32_t formatCount;
    // vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, swapchainSupportDetails.surfaceFormats.data());

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
    if (formatCount != 0) {
        swapchainSupportDetails.surfaceFormats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface,
                                             &formatCount,
                                             swapchainSupportDetails.surfaceFormats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);
    if (presentModeCount != 0) {
        swapchainSupportDetails.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface,
                                                  &presentModeCount,
                                                  swapchainSupportDetails.presentModes.data());
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

VulkanSwapchain SwapchainBuilder::build() {
    VulkanSwapchain result = {};
  
    SwapchainSupportDetails swapchainSupportDetails = querySwapchainSupport(physicalDevice, surface);

    VkSurfaceFormatKHR surfaceFormat =
        chooseSwapSurfaceFormat(swapchainSupportDetails.surfaceFormats);
    VkPresentModeKHR presentMode =
        chooseSwapPresentMode(swapchainSupportDetails.presentModes);
    VkExtent2D extent = chooseSwapExtent(swapchainSupportDetails.surfaceCapabilities, pwindow);
    
    uint32_t imageCount = swapchainSupportDetails.surfaceCapabilities.minImageCount + 1;
    if (swapchainSupportDetails.surfaceCapabilities.maxImageCount > 0 && imageCount > swapchainSupportDetails.surfaceCapabilities.maxImageCount) {
        imageCount = swapchainSupportDetails.surfaceCapabilities.maxImageCount;
    }
    
    if(swapchainSupportDetails.surfaceFormats.empty() || swapchainSupportDetails.presentModes.empty()) {
        throw std::runtime_error("current physical device does not support swapchain");
    }

    VkSwapchainCreateInfoKHR swapchainCreateInfo {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = surface,
        .minImageCount = imageCount,
        .imageFormat = surfaceFormat.format,
        .imageColorSpace = surfaceFormat.colorSpace,
        .imageExtent = extent,
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .preTransform = swapchainSupportDetails.surfaceCapabilities.currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = presentMode,
        .clipped = VK_TRUE,
        .oldSwapchain = VK_NULL_HANDLE
    };

    QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface);
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

    if (vkCreateSwapchainKHR(logicalDevice, &swapchainCreateInfo, nullptr, &result.swapchain) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain!");
    }

    vkGetSwapchainImagesKHR(logicalDevice, result.swapchain, &imageCount, nullptr);
    result.swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(logicalDevice, result.swapchain, &imageCount, result.swapChainImages.data());

    result.format = surfaceFormat.format;
    result.extent = extent;
    
    return result;
}
