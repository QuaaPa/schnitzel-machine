#ifndef SM_RHI_SWAPCHAIN_H_
#define SM_RHI_SWAPCHAIN_H_

#include <vector>
#include <vulkan/vulkan_core.h>

#include "RHI/Adapter.h"
#include "RHI/Queue.h"

namespace SM {
    struct SwapchainOptions {
        VkFormat format;
        VkColorSpaceKHR colorSpace;
        uint32_t imageCount;
        VkExtent2D imageExtent;
        uint32_t imageLayers;
        VkImageUsageFlags imageUsageFlags;
        VkSharingMode imageSharingMode;
        VkSurfaceTransformFlagBitsKHR transform;
        VkCompositeAlphaFlagBitsKHR compositeAlpha;
        VkPresentModeKHR presentMode;
        bool clipped;
    };
    
    class Swapchain {
    public:
        void initialize(const SM::Adapter &adapter, const VkDevice &deviceHandle, const std::vector<SM::Queue> &queues, const VkSurfaceKHR &surfaceHandle, const SM::SwapchainOptions &options);

        VkSwapchainKHR getHandle() { return m_handle; }
        
        void destroy(const VkDevice &deviceHandle);
        
    private:        
        VkSwapchainKHR m_handle{ VK_NULL_HANDLE };
    };
};

#endif // SM_RHI_SWAPCHAIN_H_
