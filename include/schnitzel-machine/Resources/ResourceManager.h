#ifndef SM_RESOURCES_RESOURCESMANAGER_H_
#define SM_RESOURCES_RESOURCESMANAGER_H_

#include <vulkan/vulkan_core.h>

#include "Resources/Pipeline.h"
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

        Handle<Pipeline> createGraphicsPipeline(SM::Handle<SM::ShaderModule> vertShaderModule, SM::Handle<SM::ShaderModule> fragShaderModule);
        void destroyPipeline(Handle<Pipeline> handle);
        Pipeline* get(Handle<Pipeline> handle) { return m_pipelinesPool.get(handle); }

        SM::Handle<ShaderModule> createShaderModule(const std::vector<uint32_t> &spirv, VkShaderStageFlagBits stage);
        
    private:
        VkDevice m_device;
        
        SM::Pool<Pipeline> m_pipelinesPool{ 1 };
        SM::Pool<ShaderModule> m_shaderModulesPool{ 2 };
    };
}; // namespace SM

#endif // SM_RESOURCES_RESOURCESMANAGER_H_
