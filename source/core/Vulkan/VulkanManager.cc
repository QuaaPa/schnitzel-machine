
#include "core/Vulkan/VulkanManager.h"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/trigonometric.hpp>
#include <optional>
#include <pthread.h>
#include <stdexcept>
#include <vector>
#include <chrono>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

#include "core/Vulkan/VulkanRenderPass.h"
#include "core/Vulkan/builders/FramebufferBuilder.h"
#include "core/Vulkan/builders/RenderPassBuilder.h"
#include "core/Vulkan/VulkanContext.h"
#include "core/Vulkan/builders/InstanceBuilder.h"
#include "core/Vulkan/builders/SurfaceBuilder.h"
#include "core/Vulkan/builders/DeviceBuilder.h"
#include "core/Vulkan/builders/SwapchainBuilder.h"
#include "core/Vulkan/builders/DescriptorBuilder.h"
#include "core/Vulkan/builders/PipelineBuilder.h"
#include "core/Vulkan/builders/FramebufferBuilder.h"
#include "core/Vulkan/builders/CommandBuilder.h"
#include "utils/SyncObjectsUtils.h"
#include "core/Log.h"

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
    // m_ctx.transferQueue = dev.transferQueue;
    
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
    
    m_cmd = CommandBuilder {
        .logicalDevice = m_ctx.logicalDevice,
        .graphicsQueueFamilyIndex = dev.graphicsFamilyIndex
    }.build();

    m_dscr = DescriptorBuilder {
        .logicalDevice = m_ctx.logicalDevice,
    }.build();
    
    createVertexBuffer();
    createIndexBuffer();
    createUniformBuffer();

    for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        VkDescriptorBufferInfo bufferInfo {
            .buffer = uniformBuffers[i],
            .offset = 0,
            .range = sizeof(UniformBufferObject)
        };

        VkWriteDescriptorSet descriptorWrite {
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet = m_dscr.descriptorSets[i],
            .dstBinding = 0,
            .dstArrayElement = 0,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .pBufferInfo = &bufferInfo
        };
        vkUpdateDescriptorSets(m_ctx.logicalDevice, 1, &descriptorWrite, 0, nullptr);
    }
    
    m_pipeline = PipelineBuilder {
        .logicalDevice = m_ctx.logicalDevice,
        .physicalDevice = m_ctx.physcialDevice,
        .renderPass = m_renderPass.renderPass,
        .swapchainExtent = m_swapchain.extent,
        .subpass = m_renderPass.subpass,
        .descriptorSetLayout = m_dscr.descriptorSetLayout
    }.build();

    m_framebuffer = FramebufferBuilder {
        .logicalDevice = m_ctx.logicalDevice,
        .renderPass = m_renderPass.renderPass,
        .swapchainExtent = m_swapchain.extent,
        .swapchainImageViews = m_swapchain.imageViews
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

    updateUniformBuffer(currentFrame);
    
    vkResetFences(m_ctx.logicalDevice, 1, &m_inFlightFences[currentFrame]);
    vkResetCommandBuffer(m_cmd.graphicsCommandBuffers[currentFrame], /*VkCommandBufferResetFlagBits*/ 0);
    recordCommandBuffer(m_cmd.graphicsCommandBuffers[currentFrame], imageIndex);

    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    VkSubmitInfo submitInfo {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &m_imageAvailableSemaphores[currentFrame],
        .pWaitDstStageMask = waitStages,
        .commandBufferCount = 1,
        .pCommandBuffers = &m_cmd.graphicsCommandBuffers[currentFrame],
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

void sm::VulkanManager::updateUniformBuffer(uint32_t currentImage) {
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    UniformBufferObject ubo {
        .model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
        .view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
        .proj = glm::perspective(glm::radians(45.0f), m_swapchain.extent.width / (float) m_swapchain.extent.height, 0.1f, 10.0f),
    };
    // fliping sigh on the scaling factor of the Y axis
    ubo.proj[1][1] *= -1;

    memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
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
    vkDestroyCommandPool(m_ctx.logicalDevice, m_cmd.graphicsCommandPool, nullptr);
    vkDestroyCommandPool(m_ctx.logicalDevice, m_cmd.transferCommandPool, nullptr);
    
    cleanupSwapChain();
    if(uniformBuffers.size() != 0 && uniformBuffersMemory.size() != 0) {
        for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroyBuffer(m_ctx.logicalDevice, uniformBuffers[i], nullptr);
            vkFreeMemory(m_ctx.logicalDevice, uniformBuffersMemory[i], nullptr);    
        }
    }                                                                       

    vkDestroyDescriptorPool(m_ctx.logicalDevice, m_dscr.descriptorPool, nullptr);
    vkDestroyDescriptorSetLayout(m_ctx.logicalDevice, m_dscr.descriptorSetLayout, nullptr);
    vkDestroyPipeline(m_ctx.logicalDevice, m_pipeline.pipeline, nullptr);
    vkDestroyPipelineLayout(m_ctx.logicalDevice, m_pipeline.pipelineLayout, nullptr);
    vkDestroyRenderPass(m_ctx.logicalDevice, m_pipeline.renderPass, nullptr);
    
    vkDestroyBuffer(m_ctx.logicalDevice, m_vertexBuffer.buffer, nullptr);
    vkDestroyBuffer(m_ctx.logicalDevice, m_indicesBuffer.buffer, nullptr);
    vkFreeMemory(m_ctx.logicalDevice, m_vertexBuffer.bufferMemory, nullptr);
    vkFreeMemory(m_ctx.logicalDevice, m_indicesBuffer.bufferMemory, nullptr);
    
    vkDestroyDevice(m_ctx.logicalDevice, nullptr);
    vkDestroySurfaceKHR(m_ctx.instance, m_ctx.surface, nullptr);
    vkDestroyInstance(m_ctx.instance, nullptr);
}

void sm::VulkanManager::createVertexBuffer() {
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
    m_vertexBuffer.size = vertices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(m_ctx.logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), (size_t) bufferSize);
    vkUnmapMemory(m_ctx.logicalDevice, stagingBufferMemory);

    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_vertexBuffer.buffer, m_vertexBuffer.bufferMemory);

    copyBuffer(stagingBuffer, m_vertexBuffer.buffer, bufferSize);

    vkDestroyBuffer(m_ctx.logicalDevice, stagingBuffer, nullptr);
    vkFreeMemory(m_ctx.logicalDevice, stagingBufferMemory, nullptr);
}

void sm::VulkanManager::createIndexBuffer() {
    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();
    m_indicesBuffer.size = indices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(m_ctx.logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t) bufferSize);
    vkUnmapMemory(m_ctx.logicalDevice, stagingBufferMemory);

    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_indicesBuffer.buffer, m_indicesBuffer.bufferMemory);

    copyBuffer(stagingBuffer, m_indicesBuffer.buffer, bufferSize);

    vkDestroyBuffer(m_ctx.logicalDevice, stagingBuffer, nullptr);
    vkFreeMemory(m_ctx.logicalDevice, stagingBufferMemory, nullptr);
}

void sm::VulkanManager::createUniformBuffer() {
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);

    uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
    uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

    for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);

        vkMapMemory(m_ctx.logicalDevice, uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]);
    }
        
}

void sm::VulkanManager::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(m_ctx.logicalDevice, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }
    
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(m_ctx.logicalDevice, buffer, &memRequirements);
    std::optional<uint32_t> memIndex;
    VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
    vkGetPhysicalDeviceMemoryProperties(m_ctx.physcialDevice, &physicalDeviceMemoryProperties);
    for (uint32_t i = 0; i < physicalDeviceMemoryProperties.memoryTypeCount; i++) {
        if ((memRequirements.memoryTypeBits & (1 << i)) && (physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            memIndex = i;
        }
    }
    if(!memIndex.has_value()) {
        throw std::runtime_error("failed to find suitable memory type!");
    }

    VkMemoryAllocateInfo allocInfo{
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .allocationSize = memRequirements.size,
        .memoryTypeIndex = memIndex.value(),
    };

    if (vkAllocateMemory(m_ctx.logicalDevice, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(m_ctx.logicalDevice, buffer, bufferMemory, 0);
}

void sm::VulkanManager::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = m_cmd.graphicsCommandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(m_ctx.logicalDevice, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);
    
    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(m_ctx.graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE); // should be transfer queue
    vkQueueWaitIdle(m_ctx.graphicsQueue);

    vkFreeCommandBuffers(m_ctx.logicalDevice, m_cmd.graphicsCommandPool, 1, &commandBuffer);
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

    VkClearValue clearColor = {{{0.01f, 0.01f, 0.01f, 1.0f}}};
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

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline.pipeline);

    VkBuffer vertexBuffers[] = {m_vertexBuffer.buffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(commandBuffer, m_indicesBuffer.buffer, 0, VK_INDEX_TYPE_UINT16);

    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline.pipelineLayout, 0, 1, &m_dscr.descriptorSets[currentFrame], 0, nullptr);
    vkCmdDrawIndexed(commandBuffer, m_indicesBuffer.size, 1, 0, 0, 0);
    
    //vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(m_indicesBuffer.size), 1, 0, 0, 0);

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
