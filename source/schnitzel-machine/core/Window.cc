#include "core/Window.h"
#include "core/Log.h"

void SM::GLFWwindowDeleter::operator()(GLFWwindow* w) const noexcept {
    glfwDestroyWindow(w);
}

SM::Window::Window() : m_width(0), m_height(0) {}

void SM::Window::init(int width, int height, const char* title) {
    m_width = width;
    m_height = height;

    glfwSetErrorCallback([](int error, const char* description) {
        SM_LOG_ERROR("core/Win", "GLFW error {}: {}", error, description);
    });

    if (!glfwInit()) {
        SM_LOG_ERROR("core/Win", "glfwInit failed");
        return;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);

    GLFWwindow* raw = glfwCreateWindow(m_width, m_height, title, nullptr, nullptr);
    if (!raw) {
        SM_LOG_ERROR("core/Win", "glfwCreateWindow failed");
        return;
    }

    m_window.reset(raw);
}

bool SM::Window::shouldClose() {
    return glfwWindowShouldClose(m_window.get());
}

void SM::Window::pollEvents() {
    glfwPollEvents();
}

void SM::Window::destroy() {
    m_window.reset();
    glfwTerminate();
}
