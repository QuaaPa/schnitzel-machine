#ifndef SM_RESOURCES_RESOURCESMANAGER_H_
#define SM_RESOURCES_RESOURCESMANAGER_H_

#include <vulkan/vulkan_core.h>

#include "Resources/Pipeline.h"
#include "core/Pool.h"

namespace SM {
    class ResourceManager {
    public:
        explicit ResourceManager(VkDevice device);

        ResourceManager(const ResourceManager& other) = delete;
        ResourceManager& operator=(const ResourceManager& other) = delete;
        ResourceManager(ResourceManager&& other) = delete;
        ResourceManager& operator=(ResourceManager&& other) = delete;

        Handle<Pipeline> createGraphicsPipeline(const PipelineDescription& desc);
        void destroyPipeline(Handle<Pipeline> handle);
        Pipeline* get(Handle<Pipeline> handle) { return m_pipelines.get(handle); }

    private:
        VkDevice m_device;
        
        SM::Pool<Pipeline> m_pipelines{ 1 };
    };
}; // namespace SM

#endif // SM_RESOURCES_RESOURCESMANAGER_H_
