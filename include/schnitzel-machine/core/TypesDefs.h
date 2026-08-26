#ifndef SM_CORE_TYPEDEFS_H_
#define SM_CORE_TYPEDEFS_H_

#include <vulkan/vulkan_core.h>

namespace SM {    
    enum class WindowType {
        Win32,
        Wayland,
        X11,
        GLFW
    };
    
    struct WindowHandle {
        WindowType type;
        void* nativeHandle; // GLFWwindow*, HWND, wl_surface*, ....
    };        
}

#endif // SM_CORE_TYPEDEFS_H_
