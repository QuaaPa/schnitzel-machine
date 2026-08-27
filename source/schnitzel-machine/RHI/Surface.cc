#include "RHI/Surface.h"

#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>

#include "core/VkResultToString.h"
#include "core/TypesDefs.h"
#include "core/Macros.h"
#include "core/Log.h"

void SM::Surface::initialize(const SM::WindowHandle &window, const VkInstance &instanceHandle) {        
    if(auto result = glfwCreateWindowSurface(instanceHandle, static_cast<GLFWwindow*>(window.nativeHandle), nullptr, &m_handle); result != VK_SUCCESS) {
        SM_LOG_CRITICAL("RHI", "{}: Failed to create surface", SM::toString(result));
    }       
}

void SM::Surface::destroy(const VkInstance &instanceHandle) {
    if(m_handle != VK_NULL_HANDLE) {
        vkDestroySurfaceKHR(instanceHandle, m_handle, nullptr);        
        m_handle = VK_NULL_HANDLE;
    }
}
