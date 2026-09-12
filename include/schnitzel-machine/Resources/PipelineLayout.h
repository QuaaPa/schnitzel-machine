#ifndef SM_RESOURCES_PIPELINELAYOUT_H_
#define SM_RESOURCES_PIPELINELAYOUT_H_

#include "RHI/VkResultToString.h"
#include "core/ShaderCompiler.h"
#include <vulkan/vulkan_core.h>

#include <vector>

namespace SM {
    struct PipelineLayoutDescription {
        std::vector<VkDescriptorSetLayout> setLayouts;
        std::vector<VkPushConstantRange> pushConstantRanges;
    };
    
    class PipelineLayout {
    public:
        PipelineLayout() = default;
        
        PipelineLayout(const PipelineLayout& other) = delete;
        PipelineLayout& operator=(const PipelineLayout& other) = delete;
        PipelineLayout(PipelineLayout&& other) noexcept;
        PipelineLayout& operator=(PipelineLayout&& other) noexcept;

        SM::Result initializePipelineLayout(VkDevice vkDevice, const PipelineLayoutDescription& desc);
        void destroy();
        VkPipelineLayout getPipelineLayout() const noexcept { return m_pipelineLayouts; }
        
    private :
        VkDevice m_device { VK_NULL_HANDLE };
        VkPipelineLayout m_pipelineLayouts{ VK_NULL_HANDLE };        
    };
} // namespace SM

#endif // SM_RESOURCES_PIPELINELAYOUT_H_
