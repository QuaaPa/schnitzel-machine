#include "Resources/ResourceManager.h"
#include "Resources/Pipeline.h"
#include "Resources/ShaderModule.h"
#include "Resources/Vertex.h"

#include <vector>
#include <vulkan/vulkan_core.h>

SM::ResourceManager::ResourceManager(VkDevice device)
    :m_device(device)
{
    
}

SM::Handle<SM::Pipeline> SM::ResourceManager::createGraphicsPipeline(SM::Handle<SM::ShaderModule> vertShaderModuleHandle, SM::Handle<SM::ShaderModule> fragShaderModuleHandle) {

    VkPipelineShaderStageCreateInfo vkPipelineVertexShaderStageInfo{};
    vkPipelineVertexShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vkPipelineVertexShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vkPipelineVertexShaderStageInfo.module = m_shaderModulesPool.get(vertShaderModuleHandle)->get();
    vkPipelineVertexShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo vkPipelineFragmentShaderStageInfo{};
    vkPipelineFragmentShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vkPipelineFragmentShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    vkPipelineFragmentShaderStageInfo.module = m_shaderModulesPool.get(fragShaderModuleHandle)->get();
    vkPipelineFragmentShaderStageInfo.pName = "main";
    
    std::vector<VkPipelineShaderStageCreateInfo> vkPipelineShaderStageInfos{ vkPipelineVertexShaderStageInfo, vkPipelineFragmentShaderStageInfo};

    auto bindingDescription = SM::Vertex::getBindingDescription();
    auto attributeDescriptions = SM::Vertex::getAttributeDescriptions();
    VkPipelineVertexInputStateCreateInfo vkPipelineVertexInputStateInfo{};
    vkPipelineVertexInputStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vkPipelineVertexInputStateInfo.vertexBindingDescriptionCount = 1;
    vkPipelineVertexInputStateInfo.pVertexBindingDescriptions = &bindingDescription;
    vkPipelineVertexInputStateInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vkPipelineVertexInputStateInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

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

    // TODO
    //
    VkPipelineDepthStencilStateCreateInfo vkPipelineDepthStencilStateInfo = {};

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
    VkPipelineLayout vkPipelineLayout;
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .setLayoutCount = 0,
        .pSetLayouts = nullptr,
    };
    // if (descriptorSetLayout != VK_NULL_HANDLE) {
    //     pipelineLayoutInfo.setLayoutCount = 1;
    //     pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;
    // }
    vkCreatePipelineLayout(m_device, &pipelineLayoutInfo, nullptr, &vkPipelineLayout);    
    // TODO
    //
    std::vector<VkFormat> vkColorAttachmentFormats;

    // TODO
    //
    VkFormat vkDepthAttachmentFormat = VK_FORMAT_UNDEFINED;

    // TODO
    //
    VkFormat vkStencilAttachmentFormat = VK_FORMAT_UNDEFINED;

    GraphicsPipelineDescription desc{};
    desc.stages = vkPipelineShaderStageInfos;
    desc.vertexInput = vkPipelineVertexInputStateInfo;
    desc.inputAssembly = vkPipelineInputAssemblyStateInfo;
    desc.rasterization = vkPipelineRasterizationStateInfo;
    desc.multisample = vkPipelineMultisampleStateInfo;
    desc.depthStencil = vkPipelineDepthStencilStateInfo;
    desc.colorBlend = vkPipelineColorBlendStateInfo;
    desc.layout = vkPipelineLayout;
    desc.colorAttachmentFormats = vkColorAttachmentFormats;
    desc.depthAttachmentFormat = vkDepthAttachmentFormat;
    desc.stencilAttachmentFormat = vkStencilAttachmentFormat;

    SM::Pipeline pipeline(m_device, SM::PipelineType::GRAPHICS, desc);    
    return m_pipelinesPool.insert(pipeline);
}

SM::Handle<SM::ShaderModule> SM::ResourceManager::createShaderModule(const std::vector<uint32_t> &spirv, VkShaderStageFlagBits stage) {
    SM::ShaderModule module(m_device, spirv, stage);
    return m_shaderModulesPool.insert(module);
}
