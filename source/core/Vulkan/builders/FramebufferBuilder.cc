#include "core/Vulkan/builders/FramebufferBuilder.h"

#include <cstddef>
#include <stdexcept>

#include <vulkan/vulkan_core.h>

#include "core/Vulkan/VulkanFramebuffer.h"

VulkanFramebuffer FramebufferBuilder::build() const{
    std::vector<VkFramebuffer> framebuffers;
    framebuffers.resize(swapchainImageViews.size());
    
    for(size_t i = 0; i < swapchainImageViews.size(); i++) {
        VkImageView attachments[] = {
            swapchainImageViews[i]
        };

        VkFramebufferCreateInfo framebufferInfo{
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .renderPass = renderPass,
            .attachmentCount = 1,
            .pAttachments = attachments,
            .width = swapchainExtent.width,
            .height = swapchainExtent.height,
            .layers = 1
        };
        if(vkCreateFramebuffer(logicalDevice, &framebufferInfo, nullptr, &framebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }

    return VulkanFramebuffer {
        .framebuffers = framebuffers
    };
}
