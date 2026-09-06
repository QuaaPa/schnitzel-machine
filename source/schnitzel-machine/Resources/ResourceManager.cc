#include "Resources/ResourceManager.h"
#include "Resources/Pipeline.h"

#include <vulkan/vulkan_core.h>

SM::ResourceManager::ResourceManager(VkDevice device)
    :m_device(device)
{
    
}

SM::Handle<SM::Pipeline> SM::ResourceManager::createGraphicsPipeline(const SM::PipelineDescription& desc) {
    SM::Pipeline p;
    // pipeline initialization

    return m_pipelines.insert(p);
}
