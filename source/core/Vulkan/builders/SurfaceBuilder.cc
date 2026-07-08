#include "core/Vulkan/builders/SurfaceBuilder.h"

#include <stdexcept>

#include <vulkan/vulkan_core.h>

VkSurfaceKHR SurfaceBuilder::build() const {
    VkSurfaceKHR surface;
    if(glfwCreateWindowSurface(instance, pwindow, nullptr, &surface)) {
        throw std::runtime_error("failed to create window surface");
    }
    return surface;
}
