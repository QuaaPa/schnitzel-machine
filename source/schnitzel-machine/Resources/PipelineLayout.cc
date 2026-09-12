#include "Resources/PipelineLayout.h"

#include <cstdint>

#include <vulkan/vulkan_core.h>

#include "RHI/VkResultToString.h"

SM::PipelineLayout::PipelineLayout(PipelineLayout&& other) noexcept {
    m_pipelineLayouts = other.m_pipelineLayouts;
    m_device = other.m_device;
    other.m_pipelineLayouts = VK_NULL_HANDLE;
    other.m_device = VK_NULL_HANDLE;
}
SM::PipelineLayout& SM::PipelineLayout::operator=(SM::PipelineLayout&& other) noexcept {
    if (this != &other) {
        destroy();
        m_pipelineLayouts = other.m_pipelineLayouts;
        m_device = other.m_device;
        other.m_pipelineLayouts = VK_NULL_HANDLE;
        other.m_device = VK_NULL_HANDLE;
    }
    return *this;
}

SM::Result SM::PipelineLayout::initializePipelineLayout(VkDevice vkDevice, const PipelineLayoutDescription& desc) {
    m_device = vkDevice;
    
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;
    if (!desc.setLayouts.empty()) {
        pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(desc.setLayouts.size());
        pipelineLayoutInfo.pSetLayouts = desc.setLayouts.data();
    }
    if (!desc.pushConstantRanges.empty()) {
        pipelineLayoutInfo.pushConstantRangeCount = static_cast<uint32_t>(desc.pushConstantRanges.size());
        pipelineLayoutInfo.pPushConstantRanges = desc.pushConstantRanges.data();
    }

    return vkCreatePipelineLayout(m_device, &pipelineLayoutInfo, nullptr, &m_pipelineLayouts);
}

void SM::PipelineLayout::destroy() {
    if(m_pipelineLayouts != VK_NULL_HANDLE) {
        vkDestroyPipelineLayout(m_device, m_pipelineLayouts, nullptr);
        m_pipelineLayouts = VK_NULL_HANDLE;
    }
}
