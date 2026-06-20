#include "core/Vulkan/VulkanManager.hh"
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

    m_pipeline = PipelineBuilder{}.build();
}

void CORE::VulkanManager::destroy() {
    for(auto imageView : m_swapchain.swapchainImageViews) {
        vkDestroyImageView(m_ctx.logicalDevice, imageView, nullptr);
    }
    vkDestroySwapchainKHR(m_ctx.logicalDevice, m_swapchain.swapchain, nullptr);
    vkDestroyDevice(m_ctx.logicalDevice, nullptr);
    vkDestroySurfaceKHR(m_ctx.instance, m_ctx.surface, nullptr);
    vkDestroyInstance(m_ctx.instance, nullptr);
}
