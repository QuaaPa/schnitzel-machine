#include "core/core.h"

#include "core/window.h"

void Engine::run(int argc, char* argv[]) {
    initWindow();

    mainLoop();
    cleanup();
}

void Engine::initWindow() {
    m_pwindow = new sm::Window(600, 800, "app"); 
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
