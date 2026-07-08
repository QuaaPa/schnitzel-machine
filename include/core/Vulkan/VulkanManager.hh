#ifndef VULKAN_MANAGER_HH
#define VULKAN_MANAGER_HH

#include "VulkanContext.hh"
#include "core/Vulkan/VulkanCommand.hh"
#include "core/Vulkan/VulkanRenderPass.hh"
#include "core/Vulkan/VulkanSwapchain.hh"
#include "core/Vulkan/VulkanPipeline.hh"
#include "core/Vulkan/VulkanFramebuffer.hh"

#include <GLFW/glfw3.h>
#include <cstdint>
#include <vulkan/vulkan_core.h>

#define MAX_FRAMES_IN_FLIGHT 2

namespace sm {
    class VulkanManager {
    private:
        GLFWwindow* m_pwindow = nullptr;
        
        VulkanContext m_ctx;
        VulkanSwapchain m_swapchain;
        VulkanPipeline m_pipeline;
        VulkanFramebuffer m_framebuffer;
        VulkanCommand m_cmd;
        VulkanRenderPass m_renderPass;

        uint32_t currentFrame = 0;
        std::vector<VkSemaphore> m_imageAvailableSemaphores;
        std::vector<VkSemaphore> m_renderFinishedSemaphores;
        std::vector<VkFence> m_inFlightFences;

        
    public:
        bool framebufferResized = false;
        
        VulkanManager() = default;

        void init(const char* appName, GLFWwindow* pwindow);
        void drawFrame();
        void destroy();
    
        VulkanManager(const VulkanManager &other) = delete;
        VulkanManager(VulkanManager &&other) = delete;
        VulkanManager &operator=(const VulkanManager &other) = delete;
        VulkanManager &operator=(VulkanManager &&other) = delete;

    private:
        void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
        void recreateSwapchain();
        void cleanupSwapChain();
    };
}

#endif // VULKAN_MANAGER_HH
