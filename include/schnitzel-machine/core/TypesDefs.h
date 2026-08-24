#ifndef SM_CORE_TYPEDEFS_H_
#define SM_CORE_TYPEDEFS_H_

#include <cstdint>
#include <vulkan/vulkan_core.h>

namespace SM {    
    enum SMResult {
        SM_SUCCESS = 1,
        SM_FAILURE = 0
    };

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

    struct QueueDescription {
        const VkQueue queue;
        VkQueueFlags flags;
        uint32_t timestampValidBits;
        VkExtent3D minImageTransferGranularity;
        uint32_t familyIndex;
    };
}

#endif // SM_CORE_TYPEDEFS_H_
