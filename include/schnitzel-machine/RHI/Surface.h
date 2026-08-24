#ifndef SM_RHI_SURFACE_H_
#define SM_RHI_SURFACE_H_

#include <vulkan/vulkan_core.h>

#include "core/Macros.h"
#include "core/TypesDefs.h"

namespace SM {    
    class Surface {
    public:
        ~Surface();
        
    public:
        // TODO: not sure about the parameters of initialize() yet —
        // in the future, different platforms will need different window types
        // (e.g. GLFWwindow*, HWND, wl_surface*, ...)
        SM::SMResult initialize(const SM::WindowHandle &window, VkInstance instance);

        SM_NODISCARD VkSurfaceKHR getHandle() const { return m_handle; };

        SM::SMResult destroy();
    private:        
        VkSurfaceKHR m_handle{ VK_NULL_HANDLE };
        VkInstance m_instanceHandle{ VK_NULL_HANDLE };
    };    
}; // namespace SM

#endif // SM_RHI_INSTANCE_H_
