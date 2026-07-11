#include "core/core.h"

#include "core/window.h"

void sm::Engine::run(int argc, char* argv[]) {
    init();

    mainLoop();
    cleanup();
}

void sm::Engine::init() {
    m_pwindow = sm::Window::getInstance();
    m_pwindow->init(800, 600, "SCHNITZEL");
    
    auto win = m_pwindow->getGlfwWindow();
    m_vulkanManager.init("SCHNITZEL", win);
}

void sm::Engine::mainLoop() {
    while (!m_pwindow->shouldClose()) {
        m_pwindow->pollEvents();
        m_vulkanManager.drawFrame();
    }
}

void sm::Engine::cleanup() {
    m_vulkanManager.destroy();
    m_pwindow->destroy();
}
