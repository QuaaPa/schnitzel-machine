#include "RHI/Image.h"

#include <vulkan/vulkan_core.h>

#include "RHI/VkResultToString.h"
#include "core/Log.h"

SM::Image::Image(const VkDevice &deviceHandle, const VkImage &imageHandle, const SM::ImageDescription &imageDescription)
    : m_image(imageHandle)
{
    VkImageViewCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = m_image;
    createInfo.viewType = imageDescription.viewType;
    createInfo.format = imageDescription.format;
    createInfo.components = imageDescription.components;
    createInfo.subresourceRange = imageDescription.subresourceRange;
    
    if (auto result =
        vkCreateImageView(deviceHandle, &createInfo, nullptr, &m_imageView);
        result != VK_SUCCESS) {
        SM_LOG_ERROR("RHI/Image", "{}: Failed to create image view", SM::toString(result));
    }
}

void SM::Image::destroyImageView(const VkDevice &deviceHandle) {
    vkDestroyImageView(deviceHandle, m_imageView, nullptr);
}
