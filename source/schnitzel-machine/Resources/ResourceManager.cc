#include "Resources/ResourceManager.h"
#include "Resources/Pipeline.h"
#include "Resources/PipelineLayout.h"
#include "Resources/ShaderModule.h"
#include "Resources/Vertex.h"
#include "core/Handle.h"

#include <vector>
#include <vulkan/vulkan_core.h>

SM::ResourceManager::ResourceManager(VkDevice device)
    :m_device(device)
{
    
}

SM::Handle<SM::PipelineLayout> SM::ResourceManager::createPipelineLayout() {
    SM::PipelineLayoutDescription desc{};
    desc.setLayouts = {};
    desc.pushConstantRanges = {};
    
    return m_pipelineLayoutsPool.insert(SM::PipelineLayout(m_device, desc));
}

SM::Handle<SM::Pipeline> SM::ResourceManager::createGraphicsPipeline(SM::Handle<SM::PipelineLayout> pipelineLayoutHandle, SM::Handle<SM::ShaderModule> vertShaderModuleHandle, SM::Handle<SM::ShaderModule> fragShaderModuleHandle) {

    VkPipelineShaderStageCreateInfo vkPipelineVertexShaderStageInfo{};
    vkPipelineVertexShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vkPipelineVertexShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vkPipelineVertexShaderStageInfo.module = m_shaderModulesPool.get(vertShaderModuleHandle)->getShaderModule();
    vkPipelineVertexShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo vkPipelineFragmentShaderStageInfo{};
    vkPipelineFragmentShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vkPipelineFragmentShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    vkPipelineFragmentShaderStageInfo.module = m_shaderModulesPool.get(fragShaderModuleHandle)->getShaderModule();
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
    desc.layout = m_pipelineLayoutsPool.get(pipelineLayoutHandle)->getPipelineLayout();
    desc.colorAttachmentFormats = vkColorAttachmentFormats;
    desc.depthAttachmentFormat = vkDepthAttachmentFormat;
    desc.stencilAttachmentFormat = vkStencilAttachmentFormat;

    return m_pipelinesPool.insert(SM::Pipeline(m_device, desc));
}

SM::Handle<SM::ShaderModule> SM::ResourceManager::createShaderModule(const std::vector<uint32_t> &spirv, VkShaderStageFlagBits stage) {
    return m_shaderModulesPool.insert(SM::ShaderModule(m_device, spirv, stage));
}
