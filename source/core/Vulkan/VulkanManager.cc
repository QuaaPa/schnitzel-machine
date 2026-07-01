#include "core/Vulkan/VulkanManager.hh"
#include "core/Vulkan/builders/RenderPassBuilder.hh"
#include "core/Vulkan/VulkanContext.hh"
#include "core/Vulkan/builders/InstanceBuilder.hh"
#include "core/Vulkan/builders/SurfaceBuilder.hh"
#include "core/Vulkan/builders/DeviceBuilder.hh"
#include "core/Vulkan/builders/SwapchainBuilder.hh"
#include "core/Vulkan/builders/PipelineBuilder.hh"

#include <vulkan/vulkan_core.h>

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
}

void CORE::VulkanManager::destroy() {
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
