#ifndef VULKAN_SWAPCHAIN_HH
#define VULKAN_SWAPCHAIN_HH

#include <vulkan/vulkan_core.h>
#include <vector>

struct VulkanSwapchain {
    VkSwapchainKHR swapchain = VK_NULL_HANDLE;
    std::vector<VkImage> swapchainImages;
    std::vector<VkImageView> swapchainImageViews;
    VkExtent2D extent;
    VkFormat format;
};

#endif // VULKAN_SWAPCHAIN_HH
