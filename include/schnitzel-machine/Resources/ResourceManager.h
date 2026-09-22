#ifndef SM_RESOURCES_RESOURCESMANAGER_H_
#define SM_RESOURCES_RESOURCESMANAGER_H_

#include <cstdint>
#include <vulkan/vulkan_core.h>

#include "RHI/Adapter.h"
#include "Resources/Swapchain.h"
#include "Resources/CommandPool.h"
#include "Resources/Pipeline.h"
#include "Resources/PipelineLayout.h"
#include "Resources/ShaderModule.h"
#include "core/Handle.h"
#include "core/Pool.h"

namespace SM {
    class ResourceManager {
    public:
        explicit ResourceManager(VkDevice device);

        ResourceManager(const ResourceManager& other) = delete;
        ResourceManager& operator=(const ResourceManager& other) = delete;
        ResourceManager(ResourceManager&& other) = delete;
        ResourceManager& operator=(ResourceManager&& other) = delete;

        SM::Handle<ShaderModule> createShaderModule(const std::vector<uint32_t>& spirv, VkShaderStageFlagBits stage);
        void destroy(RequiredHandle<ShaderModule> handle);
        ShaderModule* get(Handle<ShaderModule> handle) { return m_shaderModulesPool.get(handle); }

        SM::Handle<PipelineLayout> createPipelineLayout();
        void destroy(RequiredHandle<PipelineLayout> handle);
        PipelineLayout* get(Handle<PipelineLayout> handle){ return m_pipelineLayoutsPool.get(handle); }

        Handle<Pipeline> createGraphicsPipeline(SM::RequiredHandle<SM::PipelineLayout> pipelineLayoutHandle, SM::RequiredHandle<SM::ShaderModule> vertShaderModule, SM::RequiredHandle<SM::ShaderModule> fragShaderModule);
        void destroy(RequiredHandle<Pipeline> handle);
        Pipeline* get(Handle<Pipeline> handle){ return m_pipelinesPool.get(handle); }
 
        SM::Handle<CommandPool> createCommandPool(uint32_t queueFamilyIndex);
        void destroy(RequiredHandle<CommandPool> handle);
        CommandPool* get(Handle<CommandPool> handle) { return m_commandPoolsPool.get(handle); }

        SM::Handle<Swapchain> createSwapchain(VkSurfaceKHR vkSurface, const SM::Adapter& adapter, VkExtent2D imageExtent);
        void destroy(RequiredHandle<Swapchain> handle);
        Swapchain* get(Handle<Swapchain> handle) { return m_swapchainPool.get(handle); }

    private:
        VkDevice m_device;

        SM::Pool<PipelineLayout> m_pipelineLayoutsPool{ 1 };
        SM::Pool<Pipeline> m_pipelinesPool{ 1 };
        SM::Pool<ShaderModule> m_shaderModulesPool{ 2 };
        SM::Pool<CommandPool> m_commandPoolsPool{ 1 };
        SM::Pool<Swapchain> m_swapchainPool{ 1 };
    };
}; // namespace SM

#endif // SM_RESOURCES_RESOURCESMANAGER_H_
