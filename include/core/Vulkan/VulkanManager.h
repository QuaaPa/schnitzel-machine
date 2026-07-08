#ifndef SM_CORE_VULKAN_VULKANMANAGER_H_
#define SM_CORE_VULKAN_VULKANMANAGER_H_

#include <cstdint>

#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

#include "VulkanContext.h"
#include "core/Vulkan/VulkanCommand.h"
#include "core/Vulkan/VulkanRenderPass.h"
#include "core/Vulkan/VulkanSwapchain.h"
#include "core/Vulkan/VulkanPipeline.h"
#include "core/Vulkan/VulkanFramebuffer.h"

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

#endif // SM_CORE_VULKAN_VULKANMANAGER_H_
