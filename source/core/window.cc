#include <GLFW/glfw3.h>
#include "core/window.hh"


CORE::Window::Window(int width, int height, const char *title)
    : m_width(width), m_height(height) {

    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_pwindow = glfwCreateWindow(m_width, m_height, title, nullptr, nullptr);

    vulkanManager.init("SCHNITZEL", m_pwindow);
}

int CORE::Window::shouldClose() {
    return !glfwWindowShouldClose(m_pwindow);
}

void CORE::Window::pollEvents() {
    glfwPollEvents();
}

void CORE::Window::destroy() {
    vulkanManager.destroy();
  
    glfwDestroyWindow(m_pwindow);
  
    glfwTerminate();
}
