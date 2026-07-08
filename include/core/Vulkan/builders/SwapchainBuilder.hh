#ifndef SWAPCHAIN_BUILDER_HH
#define SWAPCHAIN_BUILDER_HH

#include <vulkan/vulkan_core.h>
#include "core/Vulkan/VulkanSwapchain.hh"
#include <GLFW/glfw3.h>

struct SwapchainBuilder {
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice logicalDevice = VK_NULL_HANDLE;
    VkSurfaceKHR surface = VK_NULL_HANDLE;
    GLFWwindow* pwindow = nullptr;
    VkExtent2D windowExtent = {0, 0};

    VulkanSwapchain build() const;
};

#endif // SWAPCHAIN_BUILDER_HH
