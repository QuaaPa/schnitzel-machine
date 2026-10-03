#include "core/core.h"

#include <filesystem>
#include <memory>

#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

#include "RHI/Device.h"
#include "RHI/VulkanRHI.h"
#include "Renderer/Renderer.h"
#include "core/Log.h"
#include "core/Macros.h"
#include "core/ShaderCompiler.h"
#include "core/TypesDefs.h"
#include "core/Window.h"

void SM::Engine::run(int argc, char* argv[]) {
    namespace fs = std::filesystem;
    SM_LOG_INITIALIZE("SM_LOGGER");
    SM_LOG_INFO("CORE", "Engine running...");

    auto exeDir = fs::absolute(argv[0]).lexically_normal().parent_path();
    if (!fs::is_directory(exeDir) || fs::is_empty(exeDir)) {
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

    // Window/Platform init
    // TODO: Different surface by different WindowType
    // TODO: Framebuffer resizing
    win = SM::Window::getInstance();
    win->init(800, 600, "SCHNITZEL");

    // RHI building
    // Render Hardware Interface - (instance, device, surface, queues);
    rhi = std::make_shared<SM::VulkanRHI>(VK_API_VERSION_1_3,
                                          SM::WindowHandle{
                                              SM::WindowType::GLFW,
                                              win->getGlfwWindow() });

    compiler = std::make_unique<SM::ShaderCompiler>();
    compiler->SetOptimizationLevel(shaderc_optimization_level_performance);
#ifdef SM_BUILD_DEBUG_MODE
    compiler->SetGenerateDebugInfo(true);
    compiler->SetOptimizationLevel(shaderc_optimization_level_zero); // easier to debug in RenderDoc
#endif

    auto vertShader = compiler->CompileFromFile(resourcePath / "shaders/shader.vert", SM::ShaderStage::Vertex);
    SM_LOG_DEBUG("CORE", "Vertex shader compiled successfully:{}{}", vertShader.success, vertShader.errorMessage.empty() ? "" : ", message: " + vertShader.errorMessage);

    auto fragShader = compiler->CompileFromFile(resourcePath / "shaders/shader.frag", SM::ShaderStage::Fragment);
    SM_LOG_DEBUG("CORE", "Fragment shader compiled successfully:{}{}", fragShader.success, fragShader.errorMessage.empty() ? "" : ", message: " + fragShader.errorMessage);

    renderer = std::make_unique<SM::Renderer>(rhi, vertShader, fragShader);
}

void SM::Engine::mainLoop() {
    SM_LOG_INFO("CORE", "Engine starting...");
    while (!win->shouldClose()) {
        win->pollEvents();

        renderer->beginFrame();
        
        renderer->drawExample();
        
        renderer->endFrame();
    };
}

void SM::Engine::cleanup() {
    SM_LOG_INFO("CORE", "Engine destroying...");
    rhi->device()->waitIdle();

    renderer->destroy();
    rhi->destroy();
    win->destroy();
}
