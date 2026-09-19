#ifndef SM_RHI_IMAGE_H_
#define SM_RHI_IMAGE_H_

#include <vulkan/vulkan_core.h>

#include "RHI/ImageDescription.h"
#include "RHI/VkResultToString.h"

namespace SM {

    static SM::Result getImages(VkDevice vkDevice, VkSwapchainKHR vkSwapchain, uint32_t *pSwapchainImageCount, VkImage *pSwapchainImages) {       
        return vkGetSwapchainImagesKHR(vkDevice, vkSwapchain, pSwapchainImageCount, pSwapchainImages);
    }

    struct Image {
    public:
        Image(const VkDevice &deviceHandle, const VkImage &imageHandle, const SM::ImageDescription &imageDescription);
        void destroyImageView(const VkDevice &deviceHandle);

        VkImage getImage() const noexcept { return m_image; }
        VkImageView getImageView() const noexcept { return m_imageView; }

    private:
        VkImage m_image;
        VkImageView m_imageView;
        
    };
} // namespace SM
#endif // SM_RHI_IMAGE_H_
