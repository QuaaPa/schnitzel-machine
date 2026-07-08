#ifndef SM_CORE_VULKAN_BUILDERS_SURFACEBUILDER_H_
#define SM_CORE_VULKAN_BUILDERS_SURFACEBUILDER_H_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

struct SurfaceBuilder {
    VkInstance instance = VK_NULL_HANDLE;
    GLFWwindow *pwindow = nullptr;

    VkSurfaceKHR build() const;
};

#endif // SM_CORE_VULKAN_BUILDERS_SURFACEBUILDER_H_
