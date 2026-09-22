#ifndef SM_RESOURCES_SWAPCHAIN_H_
#define SM_RESOURCES_SWAPCHAIN_H_

#include <vector>

#include <vulkan/vulkan_core.h>

#include "RHI/VkResultToString.h"
#include "RHI/AdapterSwapchainProperties.h"

namespace SM {
    struct SwapchainDescription {
        std::vector<uint32_t> queueFamilyIndices;
        VkFormat format = VK_FORMAT_B8G8R8A8_UNORM;       
        VkColorSpaceKHR colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
        uint32_t imageCount = 4;
        VkExtent2D imageExtent; // required
        uint32_t imageLayers = 1;
        VkImageUsageFlags imageUsageFlags = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        VkSharingMode imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        VkSurfaceTransformFlagBitsKHR transform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
        VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        VkPresentModeKHR presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
        bool clipped = true;
    };

    class Swapchain {
    public:
        Swapchain() = default;
        ~Swapchain();

        Swapchain(const Swapchain&) = delete;
        Swapchain& operator=(const Swapchain&) = delete;
        Swapchain(Swapchain&& other) noexcept;
        Swapchain& operator=(Swapchain&& other) noexcept;

        SM::Result initializeSwapchain(VkDevice vkDevice, VkSurfaceKHR vkSurface, const SM::AdapterSwapchainProperties& supportedSwapchainProperties, const SwapchainDescription& desc);
        VkSwapchainKHR getSwapchain() const noexcept { return m_swapchain; }

        std::vector<VkImage> getImages() const noexcept { return m_images; }
        std::vector<VkImageView> getImageViews() const noexcept { return m_imageViews; }
        VkFormat getImageFormat() const noexcept { return m_imageFormat; }
        VkExtent2D getImageExtent() const noexcept { return m_imageExtent; }

    private:
        VkDevice m_device{ VK_NULL_HANDLE };
        VkSwapchainKHR m_swapchain{ VK_NULL_HANDLE };

        std::vector<VkImage> m_images{ VK_NULL_HANDLE };
        std::vector<VkImageView> m_imageViews{ VK_NULL_HANDLE };
        VkFormat m_imageFormat{ VK_FORMAT_UNDEFINED };
        VkExtent2D m_imageExtent;
    };
}; // namespace SM

#endif // SM_RESOURCES_SWAPCHAIN_H_
