#include "core/Vulkan/VulkanManager.h"
#include "core/Vulkan/VulkanRenderPass.h"
#include "core/Vulkan/builders/FramebufferBuilder.h"
#include "core/Vulkan/builders/RenderPassBuilder.h"
#include "core/Vulkan/VulkanContext.h"
#include "core/Vulkan/builders/InstanceBuilder.h"
#include "core/Vulkan/builders/SurfaceBuilder.h"
#include "core/Vulkan/builders/DeviceBuilder.h"
#include "core/Vulkan/builders/SwapchainBuilder.h"
#include "core/Vulkan/builders/PipelineBuilder.h"
#include "core/Vulkan/builders/FramebufferBuilder.h"
#include "core/Vulkan/builders/CommandBuilder.h"
#include "utils/SyncObjectsUtils.h"

#include <GLFW/glfw3.h>
#include <cstddef>
#include <cstdint>
#include <pthread.h>
#include <vulkan/vulkan_core.h>
#include <stdexcept>

void sm::VulkanManager::init(const char* appName, GLFWwindow* pwindow) {
    m_pwindow = pwindow;
    
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
      .pwindow = m_pwindow,
      .windowExtent = {800, 600}
    }.build();

    m_renderPass = RenderPassBuilder {
        .logicalDevice = m_ctx.logicalDevice,
        .swapchainFormat = m_swapchain.format
    }.build();
    
    m_pipeline = PipelineBuilder {
        .logicalDevice = m_ctx.logicalDevice,
        .renderPass = m_renderPass.renderPass,
        .swapchainExtent = m_swapchain.extent,
        .subpass = m_renderPass.subpass
    }.build();

    m_framebuffer = FramebufferBuilder {
        .logicalDevice = m_ctx.logicalDevice,
        .renderPass = m_renderPass.renderPass,
        .swapchainExtent = m_swapchain.extent,
        .swapchainImageViews = m_swapchain.imageViews
    }.build();

    m_cmd = CommandBuilder {
        .logicalDevice = m_ctx.logicalDevice,
        .graphicsQueueFamilyIndex = dev.graphicsFamilyIndex
    }.build();

    for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        m_inFlightFences.push_back(sm::createFence(m_ctx.logicalDevice, VK_FENCE_CREATE_SIGNALED_BIT));
        m_imageAvailableSemaphores.push_back(sm::createSemaphore(m_ctx.logicalDevice));
    }
    for(size_t i = 0; i < m_swapchain.images.size(); i++) {
        m_renderFinishedSemaphores.push_back(sm::createSemaphore(m_ctx.logicalDevice));
    }
}

void sm::VulkanManager::drawFrame() {    
    vkWaitForFences(m_ctx.logicalDevice, 1, &m_inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(m_ctx.logicalDevice, m_swapchain.swapchain, UINT64_MAX, m_imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

    if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
        framebufferResized = false;
        recreateSwapchain();
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }
    
    vkResetFences(m_ctx.logicalDevice, 1, &m_inFlightFences[currentFrame]);
    vkResetCommandBuffer(m_cmd.commandBuffers[currentFrame], /*VkCommandBufferResetFlagBits*/ 0);
    recordCommandBuffer(m_cmd.commandBuffers[currentFrame], imageIndex);

    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    VkSubmitInfo submitInfo {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &m_imageAvailableSemaphores[currentFrame],
        .pWaitDstStageMask = waitStages,
        .commandBufferCount = 1,
        .pCommandBuffers = &m_cmd.commandBuffers[currentFrame],
        .signalSemaphoreCount = 1,
        .pSignalSemaphores = &m_renderFinishedSemaphores[imageIndex]
    };

    if (vkQueueSubmit(m_ctx.graphicsQueue, 1, &submitInfo, m_inFlightFences[currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkSwapchainKHR swapChains[] = {m_swapchain.swapchain};
    VkPresentInfoKHR presentInfo {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &m_renderFinishedSemaphores[imageIndex],
        .swapchainCount = 1,
        .pSwapchains = swapChains,
        .pImageIndices = &imageIndex
    };
    vkQueuePresentKHR(m_ctx.presentQueue, &presentInfo);

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void sm::VulkanManager::destroy() {
    vkDeviceWaitIdle(m_ctx.logicalDevice);
    for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroyFence(m_ctx.logicalDevice, m_inFlightFences[i], nullptr);        
        vkDestroySemaphore(m_ctx.logicalDevice, m_imageAvailableSemaphores[i], nullptr);
    }
    for(size_t i = 0; i < m_swapchain.images.size(); i++) {
        vkDestroySemaphore(m_ctx.logicalDevice, m_renderFinishedSemaphores[i], nullptr);
    }
    vkDestroyCommandPool(m_ctx.logicalDevice, m_cmd.commandPool, nullptr);

    cleanupSwapChain();    
    vkDestroyPipeline(m_ctx.logicalDevice, m_pipeline.pipeline, nullptr);
    vkDestroyPipelineLayout(m_ctx.logicalDevice, m_pipeline.pipelineLayout, nullptr);
    vkDestroyRenderPass(m_ctx.logicalDevice, m_pipeline.renderPass, nullptr);
    
    vkDestroyDevice(m_ctx.logicalDevice, nullptr);
    vkDestroySurfaceKHR(m_ctx.instance, m_ctx.surface, nullptr);
    vkDestroyInstance(m_ctx.instance, nullptr);
}

void sm::VulkanManager::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {
    VkCommandBufferBeginInfo beginInfo {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO
    };

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = m_pipeline.renderPass;
    renderPassInfo.framebuffer = m_framebuffer.framebuffers[imageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = m_swapchain.extent;

    VkClearValue clearColor = {{{0.0f, 0.f, 0.0f, 1.0f}}};
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
void sm::VulkanManager::recreateSwapchain(){
    int width = 0, height = 0;
    glfwGetFramebufferSize(m_pwindow, &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(m_pwindow, &width, &height);
        glfwWaitEvents();
    }
    
    vkDeviceWaitIdle(m_ctx.logicalDevice);
        
    m_swapchain = SwapchainBuilder {
      .physicalDevice = m_ctx.physcialDevice,
      .logicalDevice = m_ctx.logicalDevice,
      .surface = m_ctx.surface,
      .pwindow = m_pwindow,
      .windowExtent = {800, 600}
    }.build();

    m_framebuffer = FramebufferBuilder {
        .logicalDevice = m_ctx.logicalDevice,
        .renderPass = m_renderPass.renderPass,
        .swapchainExtent = m_swapchain.extent,
        .swapchainImageViews = m_swapchain.imageViews
    }.build();
}

void sm::VulkanManager::cleanupSwapChain() {
    for(auto framebuffer : m_framebuffer.framebuffers) {
        vkDestroyFramebuffer(m_ctx.logicalDevice, framebuffer, nullptr);
    }
    
    for(auto imageView : m_swapchain.imageViews) {
        vkDestroyImageView(m_ctx.logicalDevice, imageView, nullptr);
    }

    vkDestroySwapchainKHR(m_ctx.logicalDevice, m_swapchain.swapchain, nullptr);
}
