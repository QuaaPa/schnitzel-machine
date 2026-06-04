#include "core/Vulkan/VulkanManager.hh"
#include "core/Vulkan/VulkanContext.hh"
#include "core/Vulkan/builders/InstanceBuilder.hh"
#include "core/Vulkan/builders/SurfaceBuilder.hh"
#include "core/Vulkan/builders/DeviceBuilder.hh"
#include <vulkan/vulkan_core.h>

void CORE::VulkanManager::init(const char* appName, GLFWwindow* pwindow) {
    m_ctx.instance = InstanceBuilder {
        .appName = appName,
        .validation = true
    }.build();

    m_ctx.surface = SurfaceBuilder {
        .instance = m_ctx.instance,
        .pwindow = pwindow
    }.build();

    auto dev = DeviceBuilder {
        .instance = m_ctx.instance,
        .surface = m_ctx.surface
    };
}

void CORE::VulkanManager::destroy() {
    
}
