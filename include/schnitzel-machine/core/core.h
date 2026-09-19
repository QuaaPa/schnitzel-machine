#ifndef SM_CORE_CORE_H_
#define SM_CORE_CORE_H_

#include <filesystem>
#include <memory>

#include <vulkan/vulkan.h>

#include "RHI/VulkanRHI.h"
#include "Resources/CommandPool.h"
#include "Resources/Pipeline.h"
#include "Resources/PipelineLayout.h"
#include "Resources/ResourceManager.h"
#include "Resources/ShaderModule.h"
#include "core/Handle.h"
#include "core/Window.h"
#include "core/ShaderCompiler.h"

namespace SM {
    class Engine {
    private:        
        Engine()  = default;
        ~Engine() = default;

        Window* win;
        std::unique_ptr<SM::VulkanRHI> rhi;
        std::unique_ptr<SM::ShaderCompiler> compiler;
        std::unique_ptr<SM::ResourceManager> resourceManager;

        SM::Handle<ShaderModule> vertShaderModuleH;
        SM::Handle<ShaderModule> fragShaderModuleH;
        SM::Handle<PipelineLayout> pipelineLayoutHandle;
        SM::Handle<Pipeline> pipelineHandle;
        SM::Handle<CommandPool> cmdPoolHandle;

        std::vector<VkCommandBuffer> cmdBuffers;
        // members instead of single VkSemaphore:
        std::vector<VkSemaphore> imageAvailableSemaphores; // sized to MAX_FRAMES_IN_FLIGHT
        std::vector<VkSemaphore> renderFinishedSemaphores; // sized to swapchain image count
        std::vector<VkFence> inFlightFences;               // sized to MAX_FRAMES_IN_FLIGHT
        std::vector<VkFence> imagesInFlight;               // sized to swapchain image count, init VK_NULL_HANDLE
        size_t currentFrame = 0;

    public:
        static Engine* getInstance() {
            static Engine instance;
            return &instance;
        }

        void run(int argc, char *argv[]);
    
    private:
        void init(std::filesystem::path exeDir);
    
        void mainLoop();
        void cleanup();

        Engine(const Engine&) = delete;
        Engine& operator=(const Engine&) = delete;
    };    
}

#endif // SM_CORE_CORE_H_
