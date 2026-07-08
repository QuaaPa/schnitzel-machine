#include "core/core.hh"
#include "core/window.hh"

void Engine::run(int argc, char* argv[]) {
    initWindow();

    mainLoop();
    cleanup();
}

void Engine::initWindow() {
    m_pwindow = new sm::Window(600, 800, "Vulkan"); 
}

void Engine::mainLoop() {
    while (!m_pwindow->shouldClose()) {
        m_pwindow->pollEvents();
        m_pwindow->drawFrame();
    }
}

void Engine::cleanup() {
    m_pwindow->destroy();
}
