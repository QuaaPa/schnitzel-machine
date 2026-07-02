#ifndef VULKAN_MANAGER_HH
#define VULKAN_MANAGER_HH

#include "VulkanContext.hh"
#include "core/Vulkan/VulkanSwapchain.hh"
#include "core/Vulkan/VulkanPipeline.hh"

#include <GLFW/glfw3.h>

namespace CORE {
    class VulkanManager {
    private:
        VulkanContext m_ctx;
        VulkanSwapchain m_swapchain;
        VulkanPipeline m_pipeline;
        
    public:
        VulkanManager() = default;

        void init(const char* appName, GLFWwindow* pwindow);
        void destroy();
    
        VulkanManager(const VulkanManager &other) = delete;
        VulkanManager(VulkanManager &&other) = delete;
        VulkanManager &operator=(const VulkanManager &other) = delete;
        VulkanManager &operator=(VulkanManager &&other) = delete;
    };
}

#endif // VULKAN_MANAGER_HH
