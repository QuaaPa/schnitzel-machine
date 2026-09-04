#ifndef SM_RHI_IMAGEDESCRIPTOR_H_
#define SM_RHI_IMAGEDESCRIPTOR_H_

#include <vulkan/vulkan_core.h>

namespace SM {
    struct ImageDescription {
        VkImageViewType viewType;
        VkFormat format;
        VkComponentMapping components;
        VkImageSubresourceRange subresourceRange;
    };
}; // namespace SM

#endif // SM_RHI_QUEUEDESCRIPTOR_H_
