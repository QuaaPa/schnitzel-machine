#include "core/core.hh"
#include "core/window.hh"
#include <memory>
#include <vulkan/vulkan_core.h>

void Engine::run(int argc, char* argv[]) {
    initWindow();

    mainLoop();
    cleanup();
}

void Engine::initWindow() {
    m_pwindow = new CORE::Window(800, 600, "Vulkan"); 
}

void Engine::mainLoop() {
    while (!m_pwindow->shouldClose()) {
        m_pwindow->pollEvents();
    }
}

void Engine::cleanup() {
    m_pwindow->destroy();
}
