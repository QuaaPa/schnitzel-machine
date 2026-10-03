#include "Renderer/Renderer.h"

#include <cstdint>
#include <memory>
#include <vector>

#include <vulkan/vulkan_core.h>

#include "RHI/VulkanRHI.h"
#include "Resources/ResourceManager.h"

SM::Renderer::Renderer(std::shared_ptr<VulkanRHI> const& rhi, const ShaderCompileResult& vertShader, const ShaderCompileResult& fragShader)
    : m_rhi(rhi)      
{
    
    m_resourceManager = std::make_unique<SM::ResourceManager>(m_rhi->device()->getHandle());

    // auto swapchainImageExtent = VkExtent2D{
    //     .width = win->getFramebufferSize<uint32_t>().width,
    //     .height = win->getFramebufferSize<uint32_t>().height
    // };

    auto queues = m_rhi->device()->getQueues();
    std::vector<uint32_t> queueFamilyIndicies;
    queueFamilyIndicies.reserve(queues.size());    
    for(const auto& queue : queues) {
        queueFamilyIndicies.push_back(queue.familyIndex);
    }
    auto vkSurface = m_rhi->surface()->getHandle();
    swapchainHadle = m_resourceManager->createSwapchain(vkSurface,
                                                        m_rhi->adapter()->querySwapchainProperties(vkSurface),
                                                        queueFamilyIndicies,
                                                        VkExtent2D{600, 800});
    
    // commandPoolHandle = m_resourceManager->createCommandPool( /* queueFamilyIndex */0); // Should not be hardcoded
    // commandBufferHandle = m_resourceManager->createCommandBuffer(SM::VulkanRHI::adapter);

    vertShaderModuleHandle = m_resourceManager->createShaderModule(vertShader.spirv, VK_SHADER_STAGE_VERTEX_BIT);
    fragShaderModuleHandle = m_resourceManager->createShaderModule(fragShader.spirv, VK_SHADER_STAGE_FRAGMENT_BIT);
    // Passing shader module handles to create pipeline, and obtaining pipeline handle
    pipelineLayoutHandle = m_resourceManager->createPipelineLayout();
    pipelineHandle = m_resourceManager->createGraphicsPipeline(pipelineLayoutHandle, vertShaderModuleHandle, fragShaderModuleHandle);

    commandPoolHandle = m_resourceManager->createCommandPool( /* queueFamilyIndex */0); // Should not be hardcoded

    VkCommandBufferAllocateInfo allocInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
    allocInfo.commandPool = m_resourceManager->get(commandPoolHandle)->getCommandPool(); // под ваш API
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    commandBufferHandles.resize(2);
    for(auto& buffHandle : commandBufferHandles)
        buffHandle = m_resourceManager->createCommandBuffer(commandPoolHandle);

    size_t imageCount = m_resourceManager->get(swapchainHadle)->getImages().size();

    imageAvailableSemaphores.resize(2);
    inFlightFences.resize(2);
    renderFinishedSemaphores.resize(imageCount);

    imagesInFlight.assign(imageCount, VK_NULL_HANDLE);

    auto dev = rhi->device()->getHandle();
    for (auto& s : imageAvailableSemaphores) s = SM::tryCreateSemaphore(dev);
    for (auto& s : renderFinishedSemaphores) s = SM::tryCreateSemaphore(dev);
    for (auto& f : inFlightFences) f = SM::tryCreateFence(dev, VK_FENCE_CREATE_SIGNALED_BIT);
}

void SM::Renderer::beginFrame() {
    // VkCommandBufferBeginInfo beginInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
    // vkBeginCommandBuffer(, &beginInfo);
}

void SM::Renderer::drawExample() {
    VkDevice vkDevice = m_rhi->device()->getHandle();
    auto currentCommandBuffer = m_resourceManager->get(commandBufferHandles[currentFrame])->getCommandBuffer();
    vkWaitForFences(vkDevice, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
    // vkResetFences(vkDevice, 1, &inFlightFences[currentFrame]);

    uint32_t imageIndex;
    vkAcquireNextImageKHR(vkDevice, m_resourceManager->get(swapchainHadle)->getSwapchain(),
                          UINT64_MAX, imageAvailableSemaphores[currentFrame],
                          VK_NULL_HANDLE, &imageIndex);

    if (imagesInFlight[imageIndex] != VK_NULL_HANDLE)
        vkWaitForFences(vkDevice, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
    imagesInFlight[imageIndex] = inFlightFences[currentFrame];
    vkResetFences(vkDevice, 1, &inFlightFences[currentFrame]);

    vkResetCommandBuffer(currentCommandBuffer, 0);
    //
    // Command buffer recording start here:
    //
    VkCommandBufferBeginInfo beginInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
    vkBeginCommandBuffer(currentCommandBuffer, &beginInfo);

    VkImage image = m_resourceManager->get(swapchainHadle)->getImages().at(imageIndex);
    VkImageView imageView = m_resourceManager->get(swapchainHadle)->getImageViews()[imageIndex];

    // UNDEFINED -> COLOR_ATTACHMENT_OPTIMAL
    VkImageMemoryBarrier toColor{ VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER };
    toColor.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    toColor.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    toColor.srcAccessMask = 0;
    toColor.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    toColor.image = image;
    toColor.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
    vkCmdPipelineBarrier(currentCommandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                         VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                         0, 0, nullptr, 0, nullptr, 1, &toColor);

    VkRenderingAttachmentInfo colorAttachment{ VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO };
    colorAttachment.imageView = imageView;
    colorAttachment.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.clearValue.color = { { 0.1f, 0.1f, 0.0f, 1.0f } };

    VkRenderingInfo renderingInfo{ VK_STRUCTURE_TYPE_RENDERING_INFO };
    renderingInfo.renderArea = { { 0, 0 }, m_resourceManager->get(swapchainHadle)->getImageExtent() };
    renderingInfo.layerCount = 1;
    renderingInfo.colorAttachmentCount = 1;
    renderingInfo.pColorAttachments = &colorAttachment;

    vkCmdBeginRendering(currentCommandBuffer, &renderingInfo);

    vkCmdBindPipeline(currentCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_resourceManager->get(pipelineHandle)->getPipeline());

    VkExtent2D extent = m_resourceManager->get(swapchainHadle)->getImageExtent();
    VkViewport viewport{ 0.0f, 0.0f, (float)extent.width, (float)extent.height, 0.0f, 1.0f };
    VkRect2D scissor{ { 0, 0 }, extent };
    vkCmdSetViewport(currentCommandBuffer, 0, 1, &viewport);
    vkCmdSetScissor(currentCommandBuffer, 0, 1, &scissor);

    vkCmdDraw(currentCommandBuffer, 3, 1, 0, 0);

    vkCmdEndRendering(currentCommandBuffer);

    // COLOR_ATTACHMENT_OPTIMAL -> PRESENT_SRC_KHR
    VkImageMemoryBarrier toPresent{ VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER };
    toPresent.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    toPresent.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    toPresent.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    toPresent.dstAccessMask = 0;
    toPresent.image = image;
    toPresent.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
    vkCmdPipelineBarrier(currentCommandBuffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                         VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
                         0, 0, nullptr, 0, nullptr, 1, &toPresent);

    vkEndCommandBuffer(currentCommandBuffer);
    //
    // Command buffer recording ends here;
    //

    // 4. Submit
    VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    VkSubmitInfo submitInfo{ VK_STRUCTURE_TYPE_SUBMIT_INFO };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &imageAvailableSemaphores[currentFrame];
    submitInfo.pWaitDstStageMask = &waitStage;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &currentCommandBuffer;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &renderFinishedSemaphores[imageIndex];

    // rhi->queryQueues()[0].getQueue() ->
    //    First, we have got "SM::Queue" object using RHI, and then VKQueue object
    //    This will be fixed soon, by getting specific Vk::Queue
    vkQueueSubmit(m_rhi->device()->getQueues()[0].queue, 1, &submitInfo, inFlightFences[currentFrame]);

    // 5. Present
    VkSwapchainKHR swapchain = m_resourceManager->get(swapchainHadle)->getSwapchain();
    VkPresentInfoKHR presentInfo{ VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &renderFinishedSemaphores[imageIndex];
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &swapchain;
    presentInfo.pImageIndices = &imageIndex;

    // rhi->queryQueues()[0].getQueue() ->
    //    First, we have got "SM::Queue" object using RHI, and then VKQueue object
    //    This will be fixed soon, by getting specific Vk::Queue
    vkQueuePresentKHR(m_rhi->device()->getQueues()[0].queue, &presentInfo);

    currentFrame = ((currentFrame + 1) % 2);
}

void SM::Renderer::endFrame() {
    // VkImageMemoryBarrier toPresent{ VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER };
    // toPresent.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    // toPresent.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    // toPresent.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    // toPresent.dstAccessMask = 0;
    // toPresent.image = image;
    // toPresent.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
    // vkCmdPipelineBarrier(cmdBuffers[currentFrame], VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
    //                      VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
    //                      0, 0, nullptr, 0, nullptr, 1, &toPresent);

    // vkEndCommandBuffer(cmdBuffers[currentFrame]);
}

void SM::Renderer::destroy() {

    for (int i = 0; i < 2; ++i) {
        vkDestroySemaphore(m_rhi->device()->getHandle(), imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(m_rhi->device()->getHandle(), inFlightFences[i], nullptr);
    }
    for (int i = 0; i < 4; ++i) {
        vkDestroySemaphore(m_rhi->device()->getHandle(), renderFinishedSemaphores[i], nullptr);
    }

    // Double deletion due to move assigment operator for every object
    // Pool: slot.value = T{};
    //
    for (auto& buffHandle : commandBufferHandles)
        m_resourceManager->destroy(buffHandle);
    m_resourceManager->destroy(commandPoolHandle);
    m_resourceManager->destroy(vertShaderModuleHandle);
    m_resourceManager->destroy(fragShaderModuleHandle);
    m_resourceManager->destroy(pipelineLayoutHandle);
    m_resourceManager->destroy(pipelineHandle);
    m_resourceManager->destroy(swapchainHadle);
        
}
