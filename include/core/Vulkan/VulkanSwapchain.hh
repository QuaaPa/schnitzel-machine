#ifndef VULKAN_SWAPCHAIN_HH
#define VULKAN_SWAPCHAIN_HH

#include <vulkan/vulkan_core.h>
#include <vector>

struct VulkanSwapchain {
    VkSwapchainKHR swapchain = VK_NULL_HANDLE;
    // std::vector<VkImage> images;
    // std::vector<VkImageView> imageViews;
    // VkFormat format;
    //VkExtent2D extent;
};

#endif // VULKAN_SWAPCHAIN_HH
