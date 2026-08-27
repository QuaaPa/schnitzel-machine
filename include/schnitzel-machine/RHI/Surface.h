#ifndef SM_RHI_SURFACE_H_
#define SM_RHI_SURFACE_H_

#include <vulkan/vulkan_core.h>

#include "core/TypesDefs.h"
#include "core/Macros.h"

namespace SM {    
    class Surface {        
    public:
        // TODO: not sure about the parameters of initialize() yet —
        // in the future, different platforms will need different window types
        // (e.g. GLFWwindow*, HWND, wl_surface*, ...)
        void initialize(const SM::WindowHandle &window, const VkInstance &instanceHandle);

        SM_NODISCARD VkSurfaceKHR getHandle() const noexcept { return m_handle; };

        void destroy(const VkInstance &instanceHandle);
    private:        
        VkSurfaceKHR m_handle{ VK_NULL_HANDLE };        
    };    
}; // namespace SM

#endif // SM_RHI_INSTANCE_H_
