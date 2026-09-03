#include "core/Window.h"

#include <GLFW/glfw3.h>

#include "core/Log.h"
// #include "core/Vulkan/VulkanManager.h"

// static void framebufferResizeCallback(GLFWwindow* window, int width, int height) { 
//     auto app = reinterpret_cast<sm::VulkanManager*>(glfwGetWindowUserPointer(window));
//     app->framebufferResized = true;
// }

void SM::Window::init(int width, int height, const char *title) {
    m_width = width;
    m_height = height;

    glfwSetErrorCallback([](int error, const char* description) {
        SM_LOG_ERROR("core/Win", "GLFW error {}: {}", error, description);
    });
    
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);

    m_pwindow = glfwCreateWindow(m_width, m_height, title, nullptr, nullptr);
    
    // glfwSetWindowUserPointer(m_pwindow, this);
    // glfwSetFramebufferSizeCallback(m_pwindow, framebufferResizeCallback);
}

bool SM::Window::shouldClose() {
    return glfwWindowShouldClose(m_pwindow);
}

void SM::Window::pollEvents() {
    glfwPollEvents();
}

void SM::Window::getFramebufferSize(int* pWidth, int* pHeight) {
    glfwGetFramebufferSize(m_pwindow, pWidth, pHeight);
}

void SM::Window::destroy() {
    glfwDestroyWindow(m_pwindow);
    glfwTerminate();
}
