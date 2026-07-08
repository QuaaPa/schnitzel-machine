#include "core/window.h"

#include <GLFW/glfw3.h>

#include "core/Vulkan/VulkanManager.h"

static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto app = reinterpret_cast<sm::VulkanManager*>(glfwGetWindowUserPointer(window));
    app->framebufferResized = true;
}

sm::Window::Window(int width, int height, const char *title)
    : m_width(width), m_height(height) {

    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);

    m_pwindow = glfwCreateWindow(m_width, m_height, title, nullptr, nullptr);

    glfwSetWindowUserPointer(m_pwindow, this);
    glfwSetFramebufferSizeCallback(m_pwindow, framebufferResizeCallback);
    m_vulkanManager.init("SCHNITZEL", m_pwindow);
}

bool sm::Window::shouldClose() {
    return glfwWindowShouldClose(m_pwindow);
}

void sm::Window::drawFrame() {
    m_vulkanManager.drawFrame();
}

void sm::Window::pollEvents() {
    glfwPollEvents();
}

void sm::Window::destroy() {
    m_vulkanManager.destroy();
  
    glfwDestroyWindow(m_pwindow);
  
    glfwTerminate();
}
