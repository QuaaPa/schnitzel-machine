#ifndef SM_RHI_SWAPCHAIN_H_
#define SM_RHI_SWAPCHAIN_H_

#include <vulkan/vulkan_core.h>

#include "RHI/Adapter.h"

namespace SM {
    struct SwapchainOptions {
        VkSurfaceKHR surface;
        VkFormat format{ VK_FORMAT_B8G8R8A8_UNORM };
        VkColorSpaceKHR colorSpace{ VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
        uint32_t minImageCount{ 3 };
        VkExtent2D imageExtent;
        uint32_t imageLayers{ 1 };
        VkImageUsageFlags imageUsageFlags{ VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT };
        VkSharingMode imageSharingMode{ VK_SHARING_MODE_EXCLUSIVE };
        std::vector<uint32_t> queueTypeIndices;
        VkSurfaceTransformFlagBitsKHR transform{ VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR };
        VkCompositeAlphaFlagBitsKHR compositeAlpha{ VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR };
        VkPresentModeKHR presentMode{ VK_PRESENT_MODE_MAILBOX_KHR };
        bool clipped{ true };
    };
    
    class Swapchain {
    public:
        void initialize(const SM::Adapter &adapter,  const VkDevice &deviceHandle, const SM::SwapchainOptions &options);

        VkSwapchainKHR getHandle() { return m_handle; }
        
        void destroy(const VkDevice &deviceHandle);
        
    private:        
        VkSwapchainKHR m_handle{ VK_NULL_HANDLE };
    };
};

#endif // SM_RHI_SWAPCHAIN_H_
