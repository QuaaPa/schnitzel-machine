#include "core/Vulkan/builders/SwapchainBuilder.hh"
#include "core/Vulkan/SwapchainBuilder.hh"
#include "core/Vulkan/VulkanSwapchain.hh"
#include "utils/SwapchainSupportDetails.hh"
#include <cstdint>
#include <stdexcept>
#include <vulkan/vulkan_core.h>

static SwapchainSupportDetails querySwapchainSupport(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
    SwapchainSupportDetails swapchainSupportDetails;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &swapchainSupportDetails.surfaceCapabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, swapchainSupportDetails.surfaceFormats.data());

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);
    if(presentModeCount != 0) {
        swapchainSupportDetails.presentModes.resize(formatCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, swapchainSupportDetails.presentModes.data());
    }
    
    return swapchainSupportDetails;
}

static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vecotr<VkSurfaceFormatKHR>& availableFormats) {
    // nothing to do yet
}

VulkanSwapchain SwapchainBuilder::build() {
    VulkanSwapchain swapchainResult {
        .swapchain = VK_NULL_HANDLE,
        // std::vector<VkImage> images;
        // std::vector<VkImageView> imageViews;
        // VkFormat format;
        //VkExtent2D extent;
    };

    SwapchainSupportDetails swapchainSupportDetails = querySwapchainSupport(physicalDevice, surface);
    if(swapchainSupportDetails.surfaceFormats.empty() || swapchainSupportDetails.presentModes.empty()) {
        throw std::runtime_error("current physical device does not support swapchain");
    }
    return swapchainResult;
}
