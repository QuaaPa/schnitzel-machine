#include "Resources/ResourceManager.h"

#include <cstdint>
#include <vector>

#include <vulkan/vulkan_core.h>

#include "RHI/Adapter.h"
#include "Resources/Swapchain.h"
#include "Resources/CommandPool.h"
#include "Resources/Pipeline.h"
#include "Resources/PipelineLayout.h"
#include "Resources/ShaderModule.h"
#include "Resources/Swapchain.h"
#include "Resources/Vertex.h"
#include "core/Handle.h"

SM::ResourceManager::ResourceManager(VkDevice device)
    :m_device(device)
{
    
}

SM::Handle<SM::ShaderModule> SM::ResourceManager::createShaderModule(const std::vector<uint32_t>& spirv, VkShaderStageFlagBits stage) {
    SM::ShaderModule shaderModule;
    shaderModule.initializeShaderModule(m_device, spirv, stage);
    return m_shaderModulesPool.insert(std::move(shaderModule)); // move constructor !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}

void SM::ResourceManager::destroy(RequiredHandle<ShaderModule> handle) {
    // m_shaderModulesPool.get(handle)->destroy();
    m_shaderModulesPool.remove(handle);
}

SM::Handle<SM::PipelineLayout> SM::ResourceManager::createPipelineLayout() {
    SM::PipelineLayoutDescription desc{};
    desc.setLayouts = {};
    desc.pushConstantRanges = {};

    SM::PipelineLayout pipelineLayout;
    pipelineLayout.initializePipelineLayout(m_device, desc);    
    return m_pipelineLayoutsPool.insert(std::move(pipelineLayout)); // move constructor !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}

void SM::ResourceManager::destroy(SM::RequiredHandle<SM::PipelineLayout> handle) {
    m_pipelineLayoutsPool.remove(handle);
}

SM::Handle<SM::Pipeline> SM::ResourceManager::createGraphicsPipeline(SM::RequiredHandle<SM::PipelineLayout> pipelineLayoutHandle, SM::RequiredHandle<SM::ShaderModule> vertShaderModuleHandle, SM::RequiredHandle<SM::ShaderModule> fragShaderModuleHandle) {

    VkPipelineShaderStageCreateInfo vkPipelineVertexShaderStageInfo{};
    vkPipelineVertexShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vkPipelineVertexShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vkPipelineVertexShaderStageInfo.module = m_shaderModulesPool.getRequired(vertShaderModuleHandle)->getShaderModule();
    vkPipelineVertexShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo vkPipelineFragmentShaderStageInfo{};
    vkPipelineFragmentShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vkPipelineFragmentShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    vkPipelineFragmentShaderStageInfo.module = m_shaderModulesPool.getRequired(fragShaderModuleHandle)->getShaderModule();
    vkPipelineFragmentShaderStageInfo.pName = "main";
    
    std::vector<VkPipelineShaderStageCreateInfo> vkPipelineShaderStageInfos{ vkPipelineVertexShaderStageInfo, vkPipelineFragmentShaderStageInfo};

    auto bindingDescription = SM::Vertex::getBindingDescription();
    auto attributeDescriptions = SM::Vertex::getAttributeDescriptions();

    VkPipelineVertexInputStateCreateInfo vkPipelineVertexInputStateInfo{};
    vkPipelineVertexInputStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vkPipelineVertexInputStateInfo.vertexBindingDescriptionCount = 0;
    vkPipelineVertexInputStateInfo.pVertexBindingDescriptions = nullptr;
    vkPipelineVertexInputStateInfo.vertexAttributeDescriptionCount = 0;
    vkPipelineVertexInputStateInfo.pVertexAttributeDescriptions = nullptr;
    // TODO
    //
    std::vector<VkFormat> vkColorAttachmentFormats = {VK_FORMAT_B8G8R8A8_UNORM};

    // TODO
    //
    VkFormat vkDepthAttachmentFormat = VK_FORMAT_UNDEFINED;

    // TODO
    //
    VkFormat vkStencilAttachmentFormat = VK_FORMAT_UNDEFINED;

    GraphicsPipelineDescription desc{};
    desc.stages = vkPipelineShaderStageInfos;
    desc.vertexInput = vkPipelineVertexInputStateInfo;
    desc.layout = m_pipelineLayoutsPool.getRequired(pipelineLayoutHandle)->getPipelineLayout();
    desc.colorAttachmentFormats = vkColorAttachmentFormats;
    desc.depthAttachmentFormat = vkDepthAttachmentFormat;
    desc.stencilAttachmentFormat = vkStencilAttachmentFormat;

    SM::Pipeline pipeline;
    pipeline.initializePipelineAsGraphics(m_device, desc);
    return m_pipelinesPool.insert(std::move(pipeline));
}

void SM::ResourceManager::destroy(SM::RequiredHandle<SM::Pipeline> handle) {
    m_pipelinesPool.remove(handle);
}

SM::Handle<SM::CommandPool> SM::ResourceManager::createCommandPool(uint32_t queueFamilyIndex) {
    SM::CommandPool commandPool;
    commandPool.initializeCommandPool(m_device, queueFamilyIndex);
    return m_commandPoolsPool.insert(std::move(commandPool));
}

void SM::ResourceManager::destroy(SM::RequiredHandle<SM::CommandPool> handle) {
    m_commandPoolsPool.remove(handle);
}

SM::Handle<SM::Swapchain> SM::ResourceManager::createSwapchain(VkSurfaceKHR vkSurface, const SM::Adapter& adapter, VkExtent2D imageExtent) {    
    std::vector<uint32_t> queueFamilyIndices;
    queueFamilyIndices.reserve(adapter.queryQueueFamilyProperties().size());
    for(auto queueFamily : adapter.queryQueueFamilyProperties()) {
        queueFamilyIndices.emplace_back(queueFamily.index);
    }
    
    SM::SwapchainDescription desc{};
    desc.queueFamilyIndices = queueFamilyIndices;
    desc.imageExtent = imageExtent;   

    SM::Swapchain swapchain;
    auto supportedSwapchainProperties = adapter.querySwapchainProperties(vkSurface);
    swapchain.initializeSwapchain(m_device, vkSurface, supportedSwapchainProperties, desc);
    return m_swapchainPool.insert(std::move(swapchain));
    // SwapchainDescription desc{};
    // desc.format = VK_FORMAT_B8G8R8A8_UNORM;
    // desc.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    // desc.imageCount = 4;
    // desc.imageExtent = VkExtent2D{
    //     .width = win->getFramebufferSize<uint32_t>().width,
    //     .height = win->getFramebufferSize<uint32_t>().height
    // };
    // desc.imageLayers = 1;
    // desc.imageUsageFlags = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    // desc.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    // desc.transform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    // desc.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    // desc.presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
    // desc.clipped = true;    
}

void SM::ResourceManager::destroy(SM::RequiredHandle<SM::Swapchain> handle) {
    m_swapchainPool.remove(handle);
}
