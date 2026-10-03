#ifndef SM_RESOURCES_RENDERER_H_
#define SM_RESOURCES_RENDERER_H_

#include <cstdint>
#include <memory>

#include <vulkan/vulkan_core.h>

#include "RHI/VulkanRHI.h"
#include <vector>
#include "Resources/CommandBuffer.h"
#include "Resources/ResourceManager.h"
#include "core/Handle.h"
#include "core/ShaderCompiler.h"

namespace SM {
    class Renderer {
    public:
        Renderer(std::shared_ptr<VulkanRHI> const& rhi, const ShaderCompileResult& vertShader, const ShaderCompileResult& fragShader);
        ~Renderer() = default;
        
        void beginFrame();
        void drawExample();
        void endFrame();

        void destroy();
        
    private:
        std::shared_ptr<VulkanRHI> m_rhi;
        std::unique_ptr<ResourceManager> m_resourceManager;

        SM::Handle<Swapchain> swapchainHadle;
        SM::Handle<ShaderModule> vertShaderModuleHandle;
        SM::Handle<ShaderModule> fragShaderModuleHandle;
        SM::Handle<PipelineLayout> pipelineLayoutHandle;
        SM::Handle<Pipeline> pipelineHandle;
        SM::Handle<CommandPool> commandPoolHandle;
        std::vector<SM::Handle<CommandBuffer>> commandBufferHandles;

        // members instead of single VkSemaphore:
        std::vector<VkSemaphore> imageAvailableSemaphores; // sized to MAX_FRAMES_IN_FLIGHT
        std::vector<VkSemaphore> renderFinishedSemaphores; // sized to swapchain image count
        std::vector<VkFence> inFlightFences;               // sized to MAX_FRAMES_IN_FLIGHT
        std::vector<VkFence> imagesInFlight;               // sized to swapchain image count
        size_t currentFrame = 0;
    };
}; // namespace SM

#endif // SM_RESOURCES_RENDERER_H_
