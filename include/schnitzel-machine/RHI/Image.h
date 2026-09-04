#ifndef SM_RHI_IMAGE_H_
#define SM_RHI_IMAGE_H_

#include <vulkan/vulkan_core.h>

#include "RHI/ImageDescription.h"

namespace SM {
    struct Image {
    public:
        Image(const VkDevice &deviceHandle, const VkImage &imageHandle, const SM::ImageDescription &imageDescription);
        void destroyImageView(const VkDevice &deviceHandle);
        
    private:
        VkImage m_image;
        VkImageView m_imageView;
        
    };
} // namespace SM
#endif // SM_RHI_IMAGE_H_
