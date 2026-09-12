#include "Resources/Pipeline.h"

#include <array>

#include <vulkan/vulkan_core.h>

#include "RHI/VkResultToString.h"

SM::Pipeline::~Pipeline() {
    if (m_pipeline != VK_NULL_HANDLE) {
        vkDestroyPipeline(m_device, m_pipeline, nullptr);
        m_pipeline = VK_NULL_HANDLE;
    }
    m_device = VK_NULL_HANDLE;
}

SM::Pipeline::Pipeline(SM::Pipeline&& other) noexcept {
    m_pipeline = other.m_pipeline;
    m_device = other.m_device;
    other.m_pipeline = VK_NULL_HANDLE;
    other.m_device = VK_NULL_HANDLE;
}

SM::Pipeline& SM::Pipeline::operator=(SM::Pipeline&& other) noexcept {
    if (this != &other) {
        if (m_pipeline != VK_NULL_HANDLE) {
            vkDestroyPipeline(m_device, m_pipeline, nullptr);
            m_pipeline = VK_NULL_HANDLE;
        }
        m_pipeline = other.m_pipeline;
        m_device = other.m_device;
        other.m_pipeline = VK_NULL_HANDLE;
        other.m_device = VK_NULL_HANDLE;
    }
    return *this;
}

SM::Result SM::Pipeline::initializePipelineAsGraphics(VkDevice vkDevice, const GraphicsPipelineDescription& desc) {

    m_device = vkDevice;
    
    VkPipelineRenderingCreateInfo vkPipelineRenderingInfo{};
    vkPipelineRenderingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
    vkPipelineRenderingInfo.colorAttachmentCount = static_cast<uint32_t>(desc.colorAttachmentFormats.size());
    vkPipelineRenderingInfo.pColorAttachmentFormats = desc.colorAttachmentFormats.data();
    vkPipelineRenderingInfo.depthAttachmentFormat = desc.depthAttachmentFormat;
    vkPipelineRenderingInfo.stencilAttachmentFormat = desc.stencilAttachmentFormat;

    std::array<VkDynamicState, 2> vkDynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
    VkPipelineDynamicStateCreateInfo vkPipelineDynamicStateInfo{};
    vkPipelineDynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    vkPipelineDynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(vkDynamicStates.size());
    vkPipelineDynamicStateInfo.pDynamicStates = vkDynamicStates.data();

    VkPipelineViewportStateCreateInfo vkPipelineViewportStateInfo {};
    vkPipelineViewportStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    vkPipelineViewportStateInfo.viewportCount = 1;
    vkPipelineViewportStateInfo.scissorCount = 1;
 
    VkPipelineInputAssemblyStateCreateInfo vkPipelineInputAssemblyStateInfo{};
    vkPipelineInputAssemblyStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    vkPipelineInputAssemblyStateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    vkPipelineInputAssemblyStateInfo.primitiveRestartEnable = VK_FALSE;

    VkPipelineRasterizationStateCreateInfo vkPipelineRasterizationStateInfo{};
    vkPipelineRasterizationStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    vkPipelineRasterizationStateInfo.depthClampEnable = VK_FALSE;
    vkPipelineRasterizationStateInfo.rasterizerDiscardEnable = VK_FALSE;
    vkPipelineRasterizationStateInfo.polygonMode = VK_POLYGON_MODE_FILL;
    vkPipelineRasterizationStateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
    vkPipelineRasterizationStateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    vkPipelineRasterizationStateInfo.depthBiasEnable = VK_FALSE;
    vkPipelineRasterizationStateInfo.depthBiasConstantFactor = 0.0f;
    vkPipelineRasterizationStateInfo.depthBiasClamp = 0.0f;
    vkPipelineRasterizationStateInfo.depthBiasSlopeFactor = 0.0f;
    vkPipelineRasterizationStateInfo.lineWidth = 1.0f;

    VkPipelineMultisampleStateCreateInfo vkPipelineMultisampleStateInfo{};
    vkPipelineMultisampleStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    vkPipelineMultisampleStateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    vkPipelineMultisampleStateInfo.sampleShadingEnable = VK_FALSE;
    vkPipelineMultisampleStateInfo.minSampleShading = 1.0f;
    vkPipelineMultisampleStateInfo.pSampleMask = nullptr;
    vkPipelineMultisampleStateInfo.alphaToCoverageEnable = VK_FALSE;
    vkPipelineMultisampleStateInfo.alphaToOneEnable = VK_FALSE;

    VkPipelineColorBlendAttachmentState vkPipelineColorBlendAttachmentState{};
    vkPipelineColorBlendAttachmentState.blendEnable = VK_FALSE;
    vkPipelineColorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

    VkPipelineColorBlendStateCreateInfo vkPipelineColorBlendStateInfo{};
    vkPipelineColorBlendStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    vkPipelineColorBlendStateInfo.logicOpEnable = VK_FALSE;
    vkPipelineColorBlendStateInfo.logicOp = VK_LOGIC_OP_COPY;
    vkPipelineColorBlendStateInfo.attachmentCount = 1;
    vkPipelineColorBlendStateInfo.pAttachments = &vkPipelineColorBlendAttachmentState;
    vkPipelineColorBlendStateInfo.blendConstants[0] = 0.0f; // float[4]
    vkPipelineColorBlendStateInfo.blendConstants[1] = 0.0f;
    vkPipelineColorBlendStateInfo.blendConstants[2] = 0.0f;
    vkPipelineColorBlendStateInfo.blendConstants[3] = 0.0f;

    // TODO
    //
    VkPipelineDepthStencilStateCreateInfo vkPipelineDepthStencilStateInfo = {};

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.pNext = &vkPipelineRenderingInfo;
    pipelineInfo.stageCount = static_cast<uint32_t>(desc.stages.size());
    pipelineInfo.pStages = desc.stages.data();
    pipelineInfo.pVertexInputState = &desc.vertexInput;
    pipelineInfo.pInputAssemblyState = &vkPipelineInputAssemblyStateInfo;
    pipelineInfo.pViewportState = &vkPipelineViewportStateInfo;
    pipelineInfo.pRasterizationState = &vkPipelineRasterizationStateInfo;
    pipelineInfo.pMultisampleState = &vkPipelineMultisampleStateInfo;
    pipelineInfo.pDepthStencilState = &vkPipelineDepthStencilStateInfo;
    pipelineInfo.pColorBlendState = &vkPipelineColorBlendStateInfo;
    pipelineInfo.layout = desc.layout;
    pipelineInfo.renderPass = VK_NULL_HANDLE;
    pipelineInfo.pDynamicState = &vkPipelineDynamicStateInfo;
    pipelineInfo.subpass = 0;

    return vkCreateGraphicsPipelines(m_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_pipeline);
}

SM::Result SM::Pipeline::initializePipelineAsCompute(VkDevice vkDevice, const ComputePipelineDescription& desc) {
    m_device = vkDevice;

    // TODO

    return VK_SUCCESS;
}

SM::Result SM::Pipeline::initializePipelineAsRayTracing(VkDevice vkDevice, const RayTracingPipelineDescription& desc) {
    m_device = vkDevice;

    // TODO

    return VK_SUCCESS;
}
