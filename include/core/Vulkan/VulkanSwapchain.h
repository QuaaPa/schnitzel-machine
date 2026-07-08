#ifndef VULKAN_CORE_VULKAN_SWAPCHAIN_H_
#define VULKAN_CORE_VULKAN_SWAPCHAIN_H_

#include <vulkan/vulkan_core.h>
#include <vector>

struct VulkanSwapchain {
    VkSwapchainKHR swapchain = VK_NULL_HANDLE;
    std::vector<VkImage> images{VK_NULL_HANDLE};
    std::vector<VkImageView> imageViews{VK_NULL_HANDLE};
    VkExtent2D extent;
    VkFormat format;
};

#endif // VULKAN_CORE_VULKAN_SWAPCHAIN_H_
