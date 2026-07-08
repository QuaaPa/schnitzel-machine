#ifndef SM_CORE_VULKAN_BUILDERS_SWAPCHAINBUILDER_H_
#define SM_CORE_VULKAN_BUILDERS_SWAPCHAINBUILDER_H_

#include "core/Vulkan/VulkanSwapchain.h"
#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>

struct SwapchainBuilder {
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice logicalDevice = VK_NULL_HANDLE;
    VkSurfaceKHR surface = VK_NULL_HANDLE;
    GLFWwindow* pwindow = nullptr;
    VkExtent2D windowExtent = {0, 0};

    VulkanSwapchain build() const;
};

#endif // SM_CORE_VULKAN_BUILDERS_SWAPCHAINBUILDER_H_
