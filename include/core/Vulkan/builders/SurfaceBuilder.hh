#ifndef SURFACE_BUILDER_HH
#define SURFACE_BUILDER_HH

#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

struct SurfaceBuilder {
    VkInstance instance;
    GLFWwindow *pwindow;

    VkSurfaceKHR build() const;
};

#endif // SURFACE_BUILDER_HH
