#include "core/Vulkan/VulkanManager.hh"
#include "core/Vulkan/builders/FramebufferBuilder.hh"
#include "core/Vulkan/builders/RenderPassBuilder.hh"
#include "core/Vulkan/VulkanContext.hh"
#include "core/Vulkan/builders/InstanceBuilder.hh"
#include "core/Vulkan/builders/SurfaceBuilder.hh"
#include "core/Vulkan/builders/DeviceBuilder.hh"
#include "core/Vulkan/builders/SwapchainBuilder.hh"
#include "core/Vulkan/builders/PipelineBuilder.hh"
#include "core/Vulkan/builders/FramebufferBuilder.hh"
#include "core/Vulkan/builders/CommandBuilder.hh"

#include <vulkan/vulkan_core.h>
#include <stdexcept>

void CORE::VulkanManager::init(const char* appName, GLFWwindow* pwindow) {
    m_ctx.instance = InstanceBuilder {
        .appName = appName,
        .validation = true
    }.build();

    m_ctx.surface = SurfaceBuilder {
        .instance = m_ctx.instance,
        .pwindow = pwindow
    }.build();

    auto dev = DeviceBuilder {
        .instance = m_ctx.instance,
        .surface = m_ctx.surface
    }.build();

    m_ctx.physcialDevice = dev.physicalDevice;
    m_ctx.logicalDevice = dev.logicalDevice;
    m_ctx.graphicsQueue = dev.graphicsQueue;
    m_ctx.presentQueue = dev.presentQueue;

    m_swapchain = SwapchainBuilder {
      .physicalDevice = m_ctx.physcialDevice,
      .logicalDevice = m_ctx.logicalDevice,
      .surface = m_ctx.surface,
      .pwindow = pwindow,
      .windowExtent = {800, 600}
    }.build();

    auto renderPass = RenderPassBuilder {
        .logicalDevice = m_ctx.logicalDevice,
        .swapchainFormat = m_swapchain.format
    }.build();
    
    m_pipeline = PipelineBuilder {
        .logicalDevice = m_ctx.logicalDevice,
        .renderPass = renderPass.renderPass,
        .swapchainExtent = m_swapchain.extent,
        .subpass = renderPass.subpass
    }.build();

    m_framebuffer = FramebufferBuilder {
        .logicalDevice = m_ctx.logicalDevice,
        .renderPass = renderPass.renderPass,
        .swapchainExtent = m_swapchain.extent,
        .swapchainImageViews = m_swapchain.imageViews
    }.build();

    m_cmd = CommandBuilder {
        .logicalDevice = m_ctx.logicalDevice,
        .graphicsQueueFamilyIndex = dev.graphicsFamilyIndex
    }.build();
}

void CORE::VulkanManager::drawFrame() {
    // NOTHING (yet)
}

void CORE::VulkanManager::destroy() {
    vkDestroyCommandPool(m_ctx.logicalDevice, m_cmd.commandPool, nullptr);
    
    for(auto framebuffer : m_framebuffer.framebuffers) {
        vkDestroyFramebuffer(m_ctx.logicalDevice, framebuffer, nullptr);
    }
    
    vkDestroyPipeline(m_ctx.logicalDevice, m_pipeline.pipeline, nullptr);
    vkDestroyPipelineLayout(m_ctx.logicalDevice, m_pipeline.pipelineLayout, nullptr);
    vkDestroyRenderPass(m_ctx.logicalDevice, m_pipeline.renderPass, nullptr);
    
    for(auto imageView : m_swapchain.imageViews) {
        vkDestroyImageView(m_ctx.logicalDevice, imageView, nullptr);
    }
    
    vkDestroySwapchainKHR(m_ctx.logicalDevice, m_swapchain.swapchain, nullptr);
    vkDestroyDevice(m_ctx.logicalDevice, nullptr);
    vkDestroySurfaceKHR(m_ctx.instance, m_ctx.surface, nullptr);
    vkDestroyInstance(m_ctx.instance, nullptr);
}

void CORE::VulkanManager::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = m_pipeline.renderPass;
    renderPassInfo.framebuffer = m_framebuffer.framebuffers[imageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = m_swapchain.extent;

    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline.pipeline);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) m_swapchain.extent.width;
    viewport.height = (float) m_swapchain.extent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = m_swapchain.extent;
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);            

    vkCmdDraw(commandBuffer, 3, 1, 0, 0);

    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
}
