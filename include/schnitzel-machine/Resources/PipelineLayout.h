#ifndef SM_RESOURCES_PIPELINELAYOUT_H_
#define SM_RESOURCES_PIPELINELAYOUT_H_

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
        ~PipelineLayout();
        PipelineLayout(VkDevice vkDevice, const PipelineLayoutDescription& desc);

        PipelineLayout(const PipelineLayout& other) = delete;
        PipelineLayout& operator=(const PipelineLayout& other) = delete;
        PipelineLayout(PipelineLayout&& other) noexcept;
        PipelineLayout& operator=(PipelineLayout&& other) noexcept;

        VkPipelineLayout getPipelineLayout() const noexcept { return m_pipelineLayouts; }
        
    private :
        VkDevice m_device { VK_NULL_HANDLE };
        VkPipelineLayout m_pipelineLayouts{ VK_NULL_HANDLE };        
    };
} // namespace SM

#endif // SM_RESOURCES_PIPELINELAYOUT_H_
