#include "RHI/Surface.h"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

#include "core/Macros.h"
#include "core/TypesDefs.h"
#include "core/Log.h"

SM::Surface::~Surface() {
    if (m_handle != VK_NULL_HANDLE) {
        SM_LOG_CRITICAL("RHI/Surface", "Surface was not explicitly destroyed, forcing cleanup");
        destroy();
    }
}

SM::SMResult SM::Surface::initialize(const SM::WindowHandle &window, VkInstance instance) {    
    m_instanceHandle = instance;
    
    if(glfwCreateWindowSurface(instance, static_cast<GLFWwindow*>(window.nativeHandle), nullptr, &m_handle) != VK_SUCCESS) {
        SM_LOG_CRITICAL("RHI", "Failed to create surface");
        return SM_FAILURE;
    }
        
    return SM_SUCCESS;
}

SM::SMResult SM::Surface::destroy() {
    vkDestroySurfaceKHR(m_instanceHandle, m_handle, nullptr);
    m_handle = VK_NULL_HANDLE;
    
    return SM_SUCCESS;
}
