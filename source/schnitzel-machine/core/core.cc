#include "core/core.h"

#include <filesystem>

#include <memory>
#include <vulkan/vulkan_core.h>

#include "RHI/Device.h"
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
    GLFWwin = SM::Window::getInstance();
    GLFWwin->init(800, 600, "SCHNITZEL");

    rhi->createInstance(SM::InstanceOptions {
            .applicationName = "SM_APP_NAME",
            .applicationVersion = SM_MAKE_VERSION(0, 0, 1),
            .engineVersion = SM_MAKE_VERSION(0, 0, 1),
            .extensions = {"VK_KHR_wayland_surface"},            
        });
    rhi->createSurface(GLFWwin->getGlfwWindow());    
    rhi->createDevice(SM::DeviceOptions {});
    
    
}
void SM::Engine::mainLoop() {
    SM_LOG_INFO("CORE", "Engine starting...");
    // while(true) { };
}

void SM::Engine::cleanup() {
    SM_LOG_INFO("CORE", "Engine destroying...");
    rhi->destroy();
    delete GLFWwin;
}
