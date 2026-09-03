#include "core/core.h"

#include <GLFW/glfw3.h>
#include <cstdint>
#include <filesystem>

#include <memory>
#include <vulkan/vulkan_core.h>

#include "core/ShaderCompiler.h"
#include "core/TypesDefs.h"
#include "RHI/Swapchain.h"
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
    
    //auto resourcePath = exeDir / "resource";

    // RHI building
    // Render Hardware Interface - (instance, device, surface, swapchain);

    // TODO: creationg different surface by different WindowType
    win = SM::Window::getInstance();
    win->init(800, 600, "SCHNITZEL");

    rhi = std::make_unique<SM::VulkanRHI>();
    SM::InstanceOptions instanceOpt {
        .applicationName = "SM_APP_NAME",
        .applicationVersion = SM_MAKE_VERSION(0, 0, 1),
        .engineVersion = SM_MAKE_VERSION(0, 0, 1),
        .extensions = {"VK_KHR_wayland_surface"}
    };
    SM::DeviceOptions deviceOpt {
        /// nothing needed
    };

    int winWidth, winHeight;
    win->getFramebufferSize(&winHeight, &winHeight);
    SM::SwapchainOptions swapchainOpt {
        .format = VK_FORMAT_B8G8R8A8_UNORM ,
        .colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
        .imageCount = 4, 
        .imageExtent = VkExtent2D {
            .width =  static_cast<uint32_t>(winWidth),
            .height = static_cast<uint32_t>(winHeight)
        },
        .imageLayers = 1,
        .imageUsageFlags = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .transform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,        
        .presentMode = VK_PRESENT_MODE_MAILBOX_KHR,
        .clipped = true
    };
    rhi->initialize(SM::RHIOptions {
            .apiVersion = VK_API_VERSION_1_2,
            .window = SM::WindowHandle {
                SM::WindowType::GLFW,
                win->getGlfwWindow()
            },
            .InstanceOptions = instanceOpt,
            .DeviceOptions = deviceOpt,
            .SwapchainOptions = swapchainOpt
        });

    // Any hot-reload objects will create in resource/
    
    compiler = std::make_unique<SM::ShaderCompiler>();
    compiler->SetOptimizationLevel(shaderc_optimization_level_performance);
#ifdef SM_BUILD_DEBUG_MODE
    compiler->SetGenerateDebugInfo(true);
    compiler->SetOptimizationLevel(shaderc_optimization_level_zero); // easier to debug in RenderDoc
#endif
    
}

void SM::Engine::mainLoop() {
    SM_LOG_INFO("CORE", "Engine starting...");
    // while(true) { };
}

void SM::Engine::cleanup() {
    SM_LOG_INFO("CORE", "Engine destroying...");
    rhi->deviceWaitIdle(); // wait device to become idle, before destroying
    rhi->destroy();
    win->destroy();
}
