#include "RHI/Surface.h"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

#include "RHI/VkResultToString.h"
#include "core/Log.h"
#include "core/Macros.h"
#include "core/TypesDefs.h"

SM::Surface::Surface(SM::WindowHandle windowHandle, VkInstance vkInstance)
    : m_instance(vkInstance) {
    if (auto result = glfwCreateWindowSurface(m_instance, static_cast<GLFWwindow*>(windowHandle.nativeHandle), nullptr, &m_handle); result != VK_SUCCESS) {
        SM_LOG_CRITICAL("RHI", "{}: Failed to create surface", SM::toString(result));
    }
}

SM::Surface::~Surface() {
    destroy();
}

void SM::Surface::destroy() {
    if (m_handle != VK_NULL_HANDLE) {
        vkDestroySurfaceKHR(m_instance, m_handle, nullptr);
        m_handle = VK_NULL_HANDLE;
    }
}
