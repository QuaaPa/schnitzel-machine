#include "Resources/Pipeline.h"

#include <vulkan/vulkan_core.h>

#include "RHI/VkResultToString.h"
#include "core/Log.h"


SM::Pipeline::Pipeline(const VkDevice &device, PipelineType type, const GraphicsPipelineDescription& desc) {
    switch (type) {
    case SM::PipelineType::GRAPHICS:
        initializeAsGraphicsPipeline(device, desc);
    case SM::PipelineType::COMPUTE:
        initializeAsComputePipeline(device, desc);
    case SM::PipelineType::RAY_TRACING:
        initializeAsRayTracingPipeline(device, desc);
    }
}

void SM::Pipeline::initializeAsGraphicsPipeline(const VkDevice &device, const GraphicsPipelineDescription& desc) {
    m_layout = desc.layout;
        
    VkPipelineRenderingCreateInfo renderingInfo{};
    renderingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
    renderingInfo.colorAttachmentCount = static_cast<uint32_t>(desc.colorAttachmentFormats.size());
    renderingInfo.pColorAttachmentFormats = desc.colorAttachmentFormats.data();
    renderingInfo.depthAttachmentFormat = desc.depthAttachmentFormat;
    renderingInfo.stencilAttachmentFormat = desc.stencilAttachmentFormat;

    std::array<VkDynamicState, 2> dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
    VkPipelineDynamicStateCreateInfo vkPipelineDynamicStateInfo{};
    vkPipelineDynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    vkPipelineDynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    vkPipelineDynamicStateInfo.pDynamicStates = dynamicStates.data();

    VkPipelineViewportStateCreateInfo viewportStateInfo{};
    viewportStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportStateInfo.viewportCount = 1;
    viewportStateInfo.scissorCount = 1;

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.pNext = &renderingInfo;
    pipelineInfo.stageCount = static_cast<uint32_t>(desc.stages.size());
    pipelineInfo.pStages = desc.stages.data();
    pipelineInfo.pVertexInputState = &desc.vertexInput;
    pipelineInfo.pInputAssemblyState = &desc.inputAssembly;
    pipelineInfo.pViewportState = &viewportStateInfo;
    pipelineInfo.pRasterizationState = &desc.rasterization;
    pipelineInfo.pMultisampleState = &desc.multisample;
    pipelineInfo.pDepthStencilState = &desc.depthStencil;
    pipelineInfo.pColorBlendState = &desc.colorBlend;
    pipelineInfo.layout = desc.layout;
    pipelineInfo.renderPass = VK_NULL_HANDLE;
    pipelineInfo.pDynamicState = &vkPipelineDynamicStateInfo;
    pipelineInfo.subpass = 0;

    if (auto result = vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_pipeline); result != VK_SUCCESS) {
        SM_LOG_CRITICAL("Res/Pipeline", "{}: Failed to create pipeline", SM::toString(result));
    }    
}

void SM::Pipeline::initializeAsComputePipeline(const VkDevice &device, const GraphicsPipelineDescription& desc) {
    // TODO
}

void SM::Pipeline::initializeAsRayTracingPipeline(const VkDevice &device, const GraphicsPipelineDescription& desc) {
    // TODO
}
