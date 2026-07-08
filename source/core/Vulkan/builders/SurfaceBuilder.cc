#include "core/Vulkan/builders/SurfaceBuilder.hh"
#include <vulkan/vulkan_core.h>
#include <stdexcept>

VkSurfaceKHR SurfaceBuilder::build() const {
    VkSurfaceKHR surface;
    if(glfwCreateWindowSurface(instance, pwindow, nullptr, &surface)) {
        throw std::runtime_error("failed to create window surface");
    }
    return surface;
}
