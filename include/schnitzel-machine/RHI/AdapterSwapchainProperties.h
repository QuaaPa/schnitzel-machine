#ifndef SM_RHI_ADAPTERSWAPCHAINPROPERTIES_H_
#define SM_RHI_ADAPTERSWAPCHAINPROPERTIES_H_

#include <cstdint>
#include <string>
#include <vector>

#include <vulkan/vulkan_core.h>
#include <fmt/ranges.h>

namespace SM {    
    struct SurfaceCapabilities {
        uint32_t minImageCount;
        uint32_t maxImageCount;
        VkExtent2D currentExtent;
        VkExtent2D minImageExtent;
        VkExtent2D maxImageExtent;
        uint32_t maxImageArrayLayers;
        VkSurfaceTransformFlagsKHR supportedTransforms;
        VkSurfaceTransformFlagBitsKHR currentTransform;
        VkCompositeAlphaFlagsKHR supportedCompositeAlpha;
        VkImageUsageFlags supportedUsageFlags;
    };
    
    inline std::string surfaceTransformFlagBitsToString(VkSurfaceTransformFlagBitsKHR transform)
    {
        switch (transform) {
        case VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR:
            return "Identity";
        case VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR:
            return "Rotate 90";
        case VK_SURFACE_TRANSFORM_ROTATE_180_BIT_KHR:
            return "Rotate 180";
        case VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR:
            return "Rotate 270";
        case VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_BIT_KHR:
            return "Horizontal Mirror";
        case VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR:
            return "Horizontal Mirror Rotate 90";
        case VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180_BIT_KHR:
            return "Horizontal Mirror Rotate 180";
        case VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR:
            return "Horizontal Mirror Rotate 270";
        case VK_SURFACE_TRANSFORM_INHERIT_BIT_KHR:
            return "Inherit";
        default:
            return "Unknown";
        }
    }

    inline std::string presentModeToString(VkPresentModeKHR presentMode)
    {
        switch (presentMode) {
        case VK_PRESENT_MODE_IMMEDIATE_KHR:
            return "Immediate";
        case VK_PRESENT_MODE_MAILBOX_KHR:
            return "Mailbox";
        case VK_PRESENT_MODE_FIFO_KHR:
            return "Fifo";
        case VK_PRESENT_MODE_FIFO_RELAXED_KHR:
            return "Fifo Relaxed";
        case VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR:
            return "Shared Demand Refresh";
        case VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR:
            return "Shared Continuous Refresh";
        default:
            return "Unknown";
        }
    }

    
    inline uint32_t getSuitableImageCount(const SurfaceCapabilities &capabilities)
    {
        if (capabilities.maxImageCount != 0) {
            return std::min(capabilities.minImageCount + 1, capabilities.maxImageCount);
        } else {
            return capabilities.minImageCount + 1;
        }
    }

    inline std::string surfaceCapabilitiesToString(const SurfaceCapabilities &capabilities)
    {
        const std::vector<std::string> surfaceCapabilitiesString = {
            fmt::format("- minImageCount: {}", capabilities.minImageCount),
            fmt::format("- maxImageCount: {}", capabilities.maxImageCount),
            fmt::format("- currentExtent: {} x {}", capabilities.currentExtent.width, capabilities.currentExtent.height),
            fmt::format("- minImageExtent: {} x {}", capabilities.minImageExtent.width, capabilities.minImageExtent.height),
            fmt::format("- maxImageExtent: {} x {}", capabilities.maxImageExtent.width, capabilities.maxImageExtent.height),
            fmt::format("- maxImageArrayLayers: {}", capabilities.maxImageArrayLayers),
            fmt::format("- supportedTransforms: {:b}", capabilities.supportedTransforms),
            fmt::format("- currentTransform: {}", SM::surfaceTransformFlagBitsToString(capabilities.currentTransform)),
            fmt::format("- supportedCompositeAlpha: {:b}", capabilities.supportedCompositeAlpha),
            fmt::format("- supportedUsageFlags: {:b}", capabilities.supportedUsageFlags),
        };
        return fmt::format("SurfaceCapabilities:\n{}", fmt::join(surfaceCapabilitiesString, "\n"));
    }
    
    struct SurfaceFormat {
        VkFormat format;
        VkColorSpaceKHR colorSpace;
    };

    struct AdapterSwapchainProperties {
        SurfaceCapabilities capabilities;
        std::vector<SurfaceFormat> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };
};

#endif // SM_RHI_ADAPTERSWAPCHAINPROPERTIES_H_
