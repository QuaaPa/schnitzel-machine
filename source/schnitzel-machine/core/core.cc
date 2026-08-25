#include "core/core.h"

#include <GLFW/glfw3.h>
#include <filesystem>

#include <memory>
#include <vulkan/vulkan_core.h>

#include "RHI/Device.h"
#include "RHI/Swapchain.h"
#include "RHI/VulkanRHI.h"
#include "core/Window.h"
#include "core/Macros.h"
#include "core/Log.h"
#include "core/Window.h"

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
    rhi = std::make_unique<VulkanRHI>();

    // TODO: creationg different surface by different WindowType
    win = SM::Window::getInstance();
    win->init(800, 600, "SCHNITZEL");

    rhi->createInstance(SM::InstanceOptions {
            .applicationName = "SM_APP_NAME",
            .applicationVersion = SM_MAKE_VERSION(0, 0, 1),
            .engineVersion = SM_MAKE_VERSION(0, 0, 1),
            .extensions = {"VK_KHR_wayland_surface"},            
        });
    rhi->selectAdapter();
    auto surface = rhi->createSurface(win->getGlfwWindow());    
    rhi->createDevice(SM::DeviceOptions {
            // nothing needed
            // it default create one queue 
        });

    rhi->createSwapchain(SM::SwapchainOptions {
            .surface = surface,
            .format = VK_FORMAT_B8G8R8A8_SRGB,
            .colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
            .imageExtent = VkExtent2D{
                .width = 800, // replace hardcoded extent 
                .height = 600 // replace hardcoded extent 
            },
            .imageUsageFlags = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .presentMode = VK_PRESENT_MODE_MAILBOX_KHR                        
        });
}
void SM::Engine::mainLoop() {
    SM_LOG_INFO("CORE", "Engine starting...");
    // while(true) { };
}

void SM::Engine::cleanup() {
    SM_LOG_INFO("CORE", "Engine destroying...");
    rhi->destroy();
    delete win;
}
