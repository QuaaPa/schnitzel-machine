#ifndef SWAPCHAIN_SUPPORT_DETAILS_HH
#define SWAPCHAIN_SUPPORT_DETAILS_HH

#include <vector>
#include <vulkan/vulkan_core.h>

struct SwapchainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

#endif // SWAPCHAIN_SUPPORT_DETAILS_HH    
