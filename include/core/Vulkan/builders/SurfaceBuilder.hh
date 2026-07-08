#ifndef SURFACE_BUILDER_HH
#define SURFACE_BUILDER_HH

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan_core.h>

struct SurfaceBuilder {
    VkInstance instance = VK_NULL_HANDLE;
    GLFWwindow *pwindow = nullptr;

    VkSurfaceKHR build() const;
};

#endif // SURFACE_BUILDER_HH
