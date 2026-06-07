#ifndef SWAPCHAIN_SUPPORT_DETAILS_HH
#define SWAPCHAIN_SUPPORT_DETAILS_HH

#include <vector>
#include <vulkan/vulkan_core.h>

struct SwapchainSupportDetails {
    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    std::vector<VkSurfaceFormatKHR> surfaceFormats;
    std::vector<VkPresentModeKHR> presentModes;
};

#endif // SWAPCHAIN_SUPPORT_DETAILS_HH    
