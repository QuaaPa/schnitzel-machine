#ifndef SWAPCHAIN_BUILDER_HH
#define SWAPCHAIN_BUILDER_HH

#include <vulkan/vulkan_core.h>
#include "core/Vulkan/VulkanSwapchain.hh"

struct SwapchainBuilder {
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice logicalDevice = VK_NULL_HANDLE;
    VkSurfaceKHR surface = VK_NULL_HANDLE;
    VkExtent2D windowExtent = {0, 0};

    VulkanSwapchain build();
};

#endif // SWAPCHAIN_BUILDER_HH
