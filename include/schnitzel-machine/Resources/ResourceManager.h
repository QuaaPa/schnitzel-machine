#ifndef SM_RESOURCES_RESOURCESMANAGER_H_
#define SM_RESOURCES_RESOURCESMANAGER_H_

#include <vulkan/vulkan_core.h>

#include "Resources/Pipeline.h"
#include "Resources/PipelineLayout.h"
#include "Resources/ShaderModule.h"
#include "core/Pool.h"

namespace SM {
    class ResourceManager {
    public:
        explicit ResourceManager(VkDevice device);

        ResourceManager(const ResourceManager& other) = delete;
        ResourceManager& operator=(const ResourceManager& other) = delete;
        ResourceManager(ResourceManager&& other) = delete;
        ResourceManager& operator=(ResourceManager&& other) = delete;

        SM::Handle<PipelineLayout> createPipelineLayout();
        void destroy(Handle<PipelineLayout> handle){ m_pipelineLayoutsPool.remove(handle); };
        PipelineLayout* get(Handle<PipelineLayout> handle){ return m_pipelineLayoutsPool.get(handle); }

        Handle<Pipeline> createGraphicsPipeline(SM::Handle<SM::PipelineLayout> pipelineLayoutHandle, SM::Handle<SM::ShaderModule> vertShaderModule, SM::Handle<SM::ShaderModule> fragShaderModule);
        void destroy(Handle<Pipeline> handle){ m_pipelinesPool.remove(handle); };;
        Pipeline* get(Handle<Pipeline> handle){ return m_pipelinesPool.get(handle); }
 
        SM::Handle<ShaderModule> createShaderModule(const std::vector<uint32_t> &spirv, VkShaderStageFlagBits stage);
        void destroy(Handle<ShaderModule> handle){ m_shaderModulesPool.remove(handle); };;
        ShaderModule* get(Handle<ShaderModule> handle){ return m_shaderModulesPool.get(handle); }
                
    private:
        VkDevice m_device;

        SM::Pool<PipelineLayout> m_pipelineLayoutsPool{ 1 };
        SM::Pool<Pipeline> m_pipelinesPool{ 1 };
        SM::Pool<ShaderModule> m_shaderModulesPool{ 2 };
    };
}; // namespace SM

#endif // SM_RESOURCES_RESOURCESMANAGER_H_
