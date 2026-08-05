#ifndef SM_CORE_VULKAN_VULKANMANAGER_H_
#define SM_CORE_VULKAN_VULKANMANAGER_H_

#include <cstdint>

#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>
#include <glm/glm.hpp>

#include "VulkanContext.h"
#include "core/Vulkan/VulkanCommand.h"
#include "core/Vulkan/VulkanDescriptor.h"
#include "core/Vulkan/VulkanRenderPass.h"
#include "core/Vulkan/VulkanSwapchain.h"
#include "core/Vulkan/VulkanPipeline.h"
#include "core/Vulkan/VulkanFramebuffer.h"
#include "core/Vulkan/VulkanBuffer.h"
#include "utils/Types.h"

#define MAX_FRAMES_IN_FLIGHT 2

namespace sm {
    class VulkanManager {
    private:
        GLFWwindow* m_pwindow = nullptr;
        
        VulkanContext m_ctx;
        VulkanSwapchain m_swapchain;
        VulkanCommand m_cmd;
        VulkanBuffer m_vertexBuffer;
        VulkanBuffer m_indicesBuffer;
        VulkanDescriptor m_dscr;
        VulkanPipeline m_pipeline;
        VulkanFramebuffer m_framebuffer;
        VulkanRenderPass m_renderPass;
        
        uint32_t currentFrame = 0;
        std::vector<VkSemaphore> m_imageAvailableSemaphores;
        std::vector<VkSemaphore> m_renderFinishedSemaphores;
        std::vector<VkFence> m_inFlightFences;
        
    private:            
        const std::vector<sm::Vertex> vertices = {
            {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
            {{-0.5f, 0.5f}, {0.0f, 1.0f, 1.0f}}
        };

        const std::vector<uint16_t> indices = {
            0, 1, 2, 2, 3, 0
        };

        struct UniformBufferObject {
            alignas(16) glm::mat4 model;
            alignas(16) glm::mat4 view;
            alignas(16) glm::mat4 proj;
        };
                   
        std::vector<VkBuffer> uniformBuffers;
        std::vector<VkDeviceMemory> uniformBuffersMemory;
        std::vector<void*> uniformBuffersMapped;
        
        void createVertexBuffer();
        void createIndexBuffer();
        void createUniformBuffer();
        
    public:
        bool framebufferResized = false;
        
        VulkanManager() = default;

        void init(const char* appName, GLFWwindow* pwindow);
        void drawFrame();
        void updateUniformBuffer(uint32_t currentImage);
        void destroy();
    
        VulkanManager(const VulkanManager &other) = delete;
        VulkanManager(VulkanManager &&other) = delete;
        VulkanManager &operator=(const VulkanManager &other) = delete;
        VulkanManager &operator=(VulkanManager &&other) = delete;

    private:
        void createDescriptorPool();
        void createBuffer(VkDeviceSize size,
                          VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
                          VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
        void recreateSwapchain();
        void cleanupSwapChain();
        
    };
}

#endif // SM_CORE_VULKAN_VULKANMANAGER_H_
