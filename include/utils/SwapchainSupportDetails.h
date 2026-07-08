#ifndef SM_UTILS_SWAPCHAINSUPPORTDETAILS_H_
#define SM_UTILS_SWAPCHAINSUPPORTDETAILS_H_

#include <vector>
#include <vulkan/vulkan_core.h>

namespace sm {
    struct SwapchainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };    
}

#endif // SM_UTILS_SWAPCHAINSUPPORTDETAILS_H_
