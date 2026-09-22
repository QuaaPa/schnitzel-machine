#include "core/core.h"

#include <GLFW/glfw3.h>
#include <cstdint>
#include <filesystem>

#include <memory>
#include <vulkan/vulkan_core.h>

#include "RHI/VkResultToString.h"
#include "Resources/ResourceManager.h"
#include "core/ShaderCompiler.h"
#include "core/TypesDefs.h"
#include "RHI/VulkanRHI.h"
#include "RHI/Instance.h"
#include "core/Window.h"
#include "core/Macros.h"
#include "core/Window.h"
#include "RHI/Device.h"
#include "core/Log.h"

void SM::Engine::run(int argc, char* argv[]) {
    namespace fs = std::filesystem;
    SM_LOG_INIT("SM_LOGGER");
    SM_LOG_INFO("CORE", "Engine running...");

    auto exeDir = fs::absolute(argv[0]).lexically_normal().parent_path();
    if(!fs::is_directory(exeDir) || fs::is_empty(exeDir)) {
        SM_LOG_WARN("CORE", "Wrong/Empty executable directory path: {}", exeDir.string());
    }
    SM_LOG_INFO("CORE", "Absolute program directory path: {}", exeDir.string());
    
    init(exeDir);

    mainLoop();
    cleanup();
}

void SM::Engine::init(std::filesystem::path exeDir) {
    SM_LOG_INFO("CORE", "Engine initialization...");
    
    auto resourcePath = exeDir / "resources";

    // RHI building
    // Render Hardware Interface - (instance, device, surface, swapchain);

    // Window/Platform init   
    
    // TODO: Different surface by different WindowType
    // TODO: Framebuffer resizing 
    win = SM::Window::getInstance();
    win->init(800, 600, "SCHNITZEL");

    rhi = std::make_unique<SM::VulkanRHI>();

    SM::InstanceOptions instanceOpt {
        .applicationName = "SM_APPLICATION",
        .applicationVersion = SM_MAKE_VERSION(0, 0, 1),
        .engineVersion = SM_MAKE_VERSION(0, 0, 1),
        .extensions = {"VK_KHR_wayland_surface"}
    };

    SM::DeviceOptions deviceOpt {
        .requestedFeatures{
            .dynamicRendering = true
        }
    };

    // SM::SwapchainOptions swapchainOpt {
    //     .format = VK_FORMAT_B8G8R8A8_UNORM ,
    //     .colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
    //     .imageCount = 4, 
    //     .imageExtent = VkExtent2D {
    //         .width =  win->getFramebufferSize<uint32_t>().width,
    //         .height = win->getFramebufferSize<uint32_t>().height
    //     },
    //     .imageLayers = 1,
    //     .imageUsageFlags = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
    //     .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
    //     .transform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
    //     .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,        
    //     .presentMode = VK_PRESENT_MODE_MAILBOX_KHR,
    //     .clipped = true
    // };

    rhi->initialize(SM::RHIOptions {
            .apiVersion = VK_API_VERSION_1_3,
            .window = SM::WindowHandle {
                SM::WindowType::GLFW,
                win->getGlfwWindow()
            },
            .InstanceOptions = instanceOpt,
            .DeviceOptions = deviceOpt,
            // .SwapchainOptions = swapchainOpt
        });
    
    // Any hot-reload objects will create in resource/
    
    compiler = std::make_unique<SM::ShaderCompiler>();
    compiler->SetOptimizationLevel(shaderc_optimization_level_performance);
#ifdef SM_BUILD_DEBUG_MODE
    compiler->SetGenerateDebugInfo(true);
    compiler->SetOptimizationLevel(shaderc_optimization_level_zero); // easier to debug in RenderDoc
#endif

    resourceManager = std::make_unique<SM::ResourceManager>(rhi->getDevice().getHandle());

    auto swapchainImageExtent = VkExtent2D{
        .width = win->getFramebufferSize<uint32_t>().width,
        .height = win->getFramebufferSize<uint32_t>().height
    };

    swapchainHadle = resourceManager->createSwapchain(rhi->getSurface().getHandle(), rhi->getAdapter(), swapchainImageExtent);
    
    auto vertShader = compiler->CompileFromFile(resourcePath / "shaders/shader.vert", SM::ShaderStage::Vertex);
    SM_LOG_DEBUG("CORE", "Vertex shader compilation status:{}{}", vertShader.success, vertShader.errorMessage.empty() ? "" : ", message: " + vertShader.errorMessage);

    auto fragShader = compiler->CompileFromFile(resourcePath / "shaders/shader.frag", SM::ShaderStage::Fragment);
    SM_LOG_DEBUG("CORE", "Fragment shader compilation status:{}{}", fragShader.success, fragShader.errorMessage.empty() ? "" : ", message: " + fragShader.errorMessage);        
    
    vertShaderModuleH = resourceManager->createShaderModule(vertShader.spirv, VK_SHADER_STAGE_VERTEX_BIT);
    fragShaderModuleH = resourceManager->createShaderModule(fragShader.spirv, VK_SHADER_STAGE_FRAGMENT_BIT);
    // Passing shader module handles to create pipeline, and obtaining pipeline handle
    pipelineLayoutHandle = resourceManager->createPipelineLayout();
    pipelineHandle = resourceManager->createGraphicsPipeline(pipelineLayoutHandle, vertShaderModuleH, fragShaderModuleH);

    cmdPoolHandle = resourceManager->createCommandPool( /* queueFamilyIndex */0); // Should not be hardcoded

    VkCommandBufferAllocateInfo allocInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
    allocInfo.commandPool = resourceManager->get(cmdPoolHandle)->getCommandPool(); // под ваш API
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    cmdBuffers.resize(2);
    for(auto& buff : cmdBuffers)
        vkAllocateCommandBuffers(rhi->getDevice().getHandle(), &allocInfo, &buff);

    size_t imageCount = resourceManager->get(swapchainHadle)->getImages().size();

    imageAvailableSemaphores.resize(2);
    inFlightFences.resize(2);
    renderFinishedSemaphores.resize(imageCount);
    
    imagesInFlight.assign(imageCount, VK_NULL_HANDLE);
    
    auto dev = rhi->getDevice().getHandle();
    for (auto& s : imageAvailableSemaphores) s = SM::tryCreateSemaphore(dev);
    for (auto& s : renderFinishedSemaphores) s = SM::tryCreateSemaphore(dev);
    for (auto& f : inFlightFences) f = SM::tryCreateFence(dev, VK_FENCE_CREATE_SIGNALED_BIT);
}

void SM::Engine::mainLoop() {
    SM_LOG_INFO("CORE", "Engine starting...");
    VkDevice vkDevice = rhi->getDevice().getHandle();
    while (!win->shouldClose()) {
        win->pollEvents();

        vkWaitForFences(vkDevice, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
        // vkResetFences(vkDevice, 1, &inFlightFences[currentFrame]);

        uint32_t imageIndex;
        vkAcquireNextImageKHR(vkDevice, resourceManager->get(swapchainHadle)->getSwapchain(),
                              UINT64_MAX, imageAvailableSemaphores[currentFrame],
                              VK_NULL_HANDLE, &imageIndex);

        if (imagesInFlight[imageIndex] != VK_NULL_HANDLE)
            vkWaitForFences(vkDevice, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
        imagesInFlight[imageIndex] = inFlightFences[currentFrame];
        vkResetFences(vkDevice, 1, &inFlightFences[currentFrame]);
        
        vkResetCommandBuffer(cmdBuffers[currentFrame], 0);
        //
        // Command buffer recording start here:
        //
        VkCommandBufferBeginInfo beginInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
        vkBeginCommandBuffer(cmdBuffers[currentFrame], &beginInfo);

        VkImage image = resourceManager->get(swapchainHadle)->getImages().at(imageIndex);
        VkImageView imageView = resourceManager->get(swapchainHadle)->getImageViews()[imageIndex];

        // UNDEFINED -> COLOR_ATTACHMENT_OPTIMAL
        VkImageMemoryBarrier toColor{ VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER };
        toColor.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        toColor.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        toColor.srcAccessMask = 0;
        toColor.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        toColor.image = image;
        toColor.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
        vkCmdPipelineBarrier(cmdBuffers[currentFrame], VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                             VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                             0, 0, nullptr, 0, nullptr, 1, &toColor);

        VkRenderingAttachmentInfo colorAttachment{ VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO };
        colorAttachment.imageView = imageView;
        colorAttachment.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.clearValue.color = { { 0.1f, 0.1f, 0.0f, 1.0f } };

        VkRenderingInfo renderingInfo{ VK_STRUCTURE_TYPE_RENDERING_INFO };
        renderingInfo.renderArea = { { 0, 0 }, resourceManager->get(swapchainHadle)->getImageExtent() };
        renderingInfo.layerCount = 1;
        renderingInfo.colorAttachmentCount = 1;
        renderingInfo.pColorAttachments = &colorAttachment;

        vkCmdBeginRendering(cmdBuffers[currentFrame], &renderingInfo);

        vkCmdBindPipeline(cmdBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, resourceManager->get(pipelineHandle)->getPipeline());

        VkExtent2D extent = resourceManager->get(swapchainHadle)->getImageExtent();
        VkViewport viewport{ 0.0f, 0.0f, (float)extent.width, (float)extent.height, 0.0f, 1.0f };
        VkRect2D scissor{ { 0, 0 }, extent };
        vkCmdSetViewport(cmdBuffers[currentFrame], 0, 1, &viewport);
        vkCmdSetScissor(cmdBuffers[currentFrame], 0, 1, &scissor);

        vkCmdDraw(cmdBuffers[currentFrame], 3, 1, 0, 0); 

        vkCmdEndRendering(cmdBuffers[currentFrame]);

        // COLOR_ATTACHMENT_OPTIMAL -> PRESENT_SRC_KHR
        VkImageMemoryBarrier toPresent{ VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER };
        toPresent.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        toPresent.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        toPresent.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        toPresent.dstAccessMask = 0;
        toPresent.image = image;
        toPresent.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
        vkCmdPipelineBarrier(cmdBuffers[currentFrame], VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                             VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
                             0, 0, nullptr, 0, nullptr, 1, &toPresent);

        vkEndCommandBuffer(cmdBuffers[currentFrame]);
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
        submitInfo.pCommandBuffers = &cmdBuffers[currentFrame];
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = &renderFinishedSemaphores[imageIndex];

        // rhi->queryQueues()[0].getQueue() ->
        //    First, we have got "SM::Queue" object using RHI, and then VKQueue object
        //    This will be fixed soon, by getting specific Vk::Queue
        vkQueueSubmit(rhi->queryQueues()[0].getQueue(), 1, &submitInfo, inFlightFences[currentFrame]);

        // 5. Present
        VkSwapchainKHR swapchain = resourceManager->get(swapchainHadle)->getSwapchain();
        VkPresentInfoKHR presentInfo{ VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = &renderFinishedSemaphores[imageIndex];
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = &swapchain;
        presentInfo.pImageIndices = &imageIndex;

        // rhi->queryQueues()[0].getQueue() ->
        //    First, we have got "SM::Queue" object using RHI, and then VKQueue object
        //    This will be fixed soon, by getting specific Vk::Queue
        vkQueuePresentKHR(rhi->queryQueues()[0].getQueue(), &presentInfo);

        currentFrame = ((currentFrame + 1) % 2);
    };
}

void SM::Engine::cleanup() {
    SM_LOG_INFO("CORE", "Engine destroying...");
    SM_LOG_DEBUG("RHI", "Waiting for a device to become idle");
    rhi->deviceWaitIdle();

    for (int i = 0; i < 2; ++i) {
        vkDestroySemaphore(rhi->getDevice().getHandle(), imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(rhi->getDevice().getHandle(), inFlightFences[i], nullptr);
    }
    for (int i = 0; i < 4; ++i) {
        vkDestroySemaphore(rhi->getDevice().getHandle(), renderFinishedSemaphores[i], nullptr);
    }

    // Double deletion due to move assigment operator for every object
    // Pool: slot.value = T{};
    //
    resourceManager->destroy(cmdPoolHandle);
    resourceManager->destroy(vertShaderModuleH);
    resourceManager->destroy(fragShaderModuleH);
    resourceManager->destroy(pipelineLayoutHandle);
    resourceManager->destroy(pipelineHandle);
    resourceManager->destroy(swapchainHadle);
    
    rhi->destroy();
    win->destroy();
}
