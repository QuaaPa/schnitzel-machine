#include "Renderer/Renderer.h"

#include <cstdint>
#include <memory>
#include <vector>

#include <vulkan/vulkan_core.h>

#include "RHI/VulkanRHI.h"
#include "Resources/ResourceManager.h"

SM::Renderer::Renderer(std::shared_ptr<VulkanRHI> const& rhi, const ShaderCompileResult& vertShader, const ShaderCompileResult& fragShader)
    : m_rhi(rhi) {
    m_resourceManager = std::make_unique<SM::ResourceManager>(m_rhi->device()->getHandle());

    auto queues = m_rhi->device()->getQueues();
    std::vector<uint32_t> queueFamilyIndicies;
    queueFamilyIndicies.reserve(queues.size());
    for (const auto& queue : queues) {
        queueFamilyIndicies.push_back(queue.familyIndex);
    }
    // TODO: temporary hardcoded extent. Proper swapchain size (from framebuffer size)
    // will be set once dynamic window/framebuffer resize handling is implemented.
    VkExtent2D swExtent = { 600, 800 };

    auto vkSurface = m_rhi->surface()->getHandle();
    swapchainHadle = m_resourceManager->createSwapchain(vkSurface,
                                                        m_rhi->adapter()->querySwapchainProperties(vkSurface),
                                                        queueFamilyIndicies,
                                                        swExtent);

    // commandPoolHandle = m_resourceManager->createCommandPool( /* queueFamilyIndex */0); // Should not be hardcoded
    // commandBufferHandle = m_resourceManager->createCommandBuffer(SM::VulkanRHI::adapter);

    vertShaderModuleHandle = m_resourceManager->createShaderModule(vertShader.spirv, VK_SHADER_STAGE_VERTEX_BIT);
    fragShaderModuleHandle = m_resourceManager->createShaderModule(fragShader.spirv, VK_SHADER_STAGE_FRAGMENT_BIT);
    // Passing shader module handles to create pipeline, and obtaining pipeline handle
    pipelineLayoutHandle = m_resourceManager->createPipelineLayout();
    pipelineHandle = m_resourceManager->createGraphicsPipeline(pipelineLayoutHandle, vertShaderModuleHandle, fragShaderModuleHandle);

    commandPoolHandle = m_resourceManager->createCommandPool(/* queueFamilyIndex */ 0); // Should not be hardcoded

    VkCommandBufferAllocateInfo allocInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
    allocInfo.commandPool = m_resourceManager->get(commandPoolHandle)->getCommandPool(); // под ваш API
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    commandBufferHandles.resize(2);
    for (auto& buffHandle : commandBufferHandles)
        buffHandle = m_resourceManager->createCommandBuffer(commandPoolHandle);

    size_t imageCount = m_resourceManager->get(swapchainHadle)->getImages().size();

    imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(imageCount);
    inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

    imagesInFlight.assign(imageCount, VK_NULL_HANDLE);

    auto dev = rhi->device()->getHandle();
    for (auto& s : imageAvailableSemaphores) s = SM::tryCreateSemaphore(dev);
    for (auto& s : renderFinishedSemaphores) s = SM::tryCreateSemaphore(dev);
    for (auto& f : inFlightFences) f = SM::tryCreateFence(dev, VK_FENCE_CREATE_SIGNALED_BIT);
}

void SM::Renderer::beginFrame() {
    m_rhi->device()->waitForFences(1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    m_resourceManager->get(swapchainHadle)->acquireNextImage(UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &m_imageIndex);

    if (imagesInFlight[m_imageIndex] != VK_NULL_HANDLE)
        m_rhi->device()->waitForFences(1, &imagesInFlight[m_imageIndex], VK_TRUE, UINT64_MAX);
    imagesInFlight[m_imageIndex] = inFlightFences[currentFrame];
    m_rhi->device()->resetFences(1, &inFlightFences[currentFrame]);

    m_resourceManager->get(commandBufferHandles[currentFrame])->resetCommandBuffer();
    //
    // Command buffer recording start here:
    //
    VkCommandBufferBeginInfo beginInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
    m_resourceManager->get(commandBufferHandles[currentFrame])->beginCommandBuffer(&beginInfo);

    VkImage image = m_resourceManager->get(swapchainHadle)->getImages().at(m_imageIndex);
    VkImageView imageView = m_resourceManager->get(swapchainHadle)->getImageViews()[m_imageIndex];

    // UNDEFINED -> COLOR_ATTACHMENT_OPTIMAL
    // TODO: change VKImageMemoryBarrier2
    VkImageMemoryBarrier toColor{ VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER };
    toColor.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    toColor.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    toColor.srcAccessMask = 0;
    toColor.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    toColor.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    toColor.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    toColor.image = image;
    toColor.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
    m_resourceManager->get(commandBufferHandles[currentFrame])->cmdPipelineBarrier(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0, 0, nullptr, 0, nullptr, 1, &toColor);

    VkRenderingAttachmentInfo colorAttachment{ VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO };
    colorAttachment.imageView = imageView;
    colorAttachment.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.clearValue.color = { { 0.0f, 0.0f, 0.0f, 1.0f } };

    VkRenderingInfo renderingInfo{ VK_STRUCTURE_TYPE_RENDERING_INFO };
    renderingInfo.renderArea = { { 0, 0 }, m_resourceManager->get(swapchainHadle)->getImageExtent() };
    renderingInfo.layerCount = 1;
    renderingInfo.colorAttachmentCount = 1;
    renderingInfo.pColorAttachments = &colorAttachment;

    m_resourceManager->get(commandBufferHandles[currentFrame])->cmdBeginRendering(&renderingInfo);

    VkExtent2D extent = m_resourceManager->get(swapchainHadle)->getImageExtent();
    VkViewport viewport{ 0.0f, 0.0f, (float)extent.width, (float)extent.height, 0.0f, 1.0f };
    VkRect2D scissor{ { 0, 0 }, extent };
    m_resourceManager->get(commandBufferHandles[currentFrame])->cmdSetViewport(0, 1, &viewport);
    m_resourceManager->get(commandBufferHandles[currentFrame])->cmdSetScissor(0, 1, &scissor);
}

void SM::Renderer::drawExample() {
    m_resourceManager->get(commandBufferHandles[currentFrame])->cmdBindPipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, m_resourceManager->get(pipelineHandle)->getPipeline());

    m_resourceManager->get(commandBufferHandles[currentFrame])->cmdDraw(3, 1, 0, 0);
}

void SM::Renderer::endFrame() {
    m_resourceManager->get(commandBufferHandles[currentFrame])->cmdEndRendering();

    VkImage image = m_resourceManager->get(swapchainHadle)->getImages().at(m_imageIndex);

    // COLOR_ATTACHMENT_OPTIMAL -> PRESENT_SRC_KHR
    VkImageMemoryBarrier toPresent{ VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER };
    toPresent.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    toPresent.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    toPresent.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    toPresent.dstAccessMask = 0;
    toPresent.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    toPresent.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    toPresent.image = image;
    toPresent.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
    m_resourceManager->get(commandBufferHandles[currentFrame])->cmdPipelineBarrier(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &toPresent);

    m_resourceManager->get(commandBufferHandles[currentFrame])->endCommandBuffer();
    //
    // Command buffer recording ends here;
    //

    // 4. Submit
    auto vkCurrentCommandBuffer = m_resourceManager->get(commandBufferHandles[currentFrame])->getCommandBuffer();
    VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    VkSubmitInfo submitInfo{ VK_STRUCTURE_TYPE_SUBMIT_INFO };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &imageAvailableSemaphores[currentFrame];
    submitInfo.pWaitDstStageMask = &waitStage;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &vkCurrentCommandBuffer;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &renderFinishedSemaphores[m_imageIndex];

    // rhi->queryQueues()[0].getQueue() ->
    //    First, we have got "SM::Queue" object using RHI, and then VKQueue object
    //    This will be fixed soon, by getting specific Vk::Queue
    vkQueueSubmit(m_rhi->device()->getQueues()[0].queue, 1, &submitInfo, inFlightFences[currentFrame]);

    // 5. Present
    VkSwapchainKHR swapchain = m_resourceManager->get(swapchainHadle)->getSwapchain();
    VkPresentInfoKHR presentInfo{ VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &renderFinishedSemaphores[m_imageIndex];
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &swapchain;
    presentInfo.pImageIndices = &m_imageIndex;

    // rhi->queryQueues()[0].getQueue() ->
    //    First, we have got "SM::Queue" object using RHI, and then VKQueue object
    //    This will be fixed soon, by getting specific Vk::Queue
    vkQueuePresentKHR(m_rhi->device()->getQueues()[0].queue, &presentInfo);

    currentFrame = ((currentFrame + 1) % 2);
}
void SM::Renderer::destroy() {

    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
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
