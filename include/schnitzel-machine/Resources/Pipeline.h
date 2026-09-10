#ifndef SM_RESOURCES_PIPELINE_H_
#define SM_RESOURCES_PIPELINE_H_

#include <vector>

#include <vulkan/vulkan_core.h>

namespace SM {
    struct GraphicsPipelineDescription {        
        std::vector<VkPipelineShaderStageCreateInfo> stages;
        VkPipelineVertexInputStateCreateInfo vertexInput;
        VkPipelineLayout layout;

        std::vector<VkFormat> colorAttachmentFormats;
        VkFormat depthAttachmentFormat = VK_FORMAT_UNDEFINED;
        VkFormat stencilAttachmentFormat = VK_FORMAT_UNDEFINED;
    };
    struct ComputePipelineDescription {
        // nothing yet
    };
    struct RayTracingPipelineDescription {
        // nothing yet
    };

    class Pipeline {
    public:
        Pipeline() = default;
        ~Pipeline();
        Pipeline(VkDevice vkDevice, const GraphicsPipelineDescription& desc);
        Pipeline(VkDevice vkDevice, const ComputePipelineDescription& desc);
        Pipeline(VkDevice vkDevice, const RayTracingPipelineDescription& desc);

        Pipeline(const Pipeline&) = delete;
        Pipeline& operator=(const Pipeline&) = delete;
        Pipeline(Pipeline&& other) noexcept;
        Pipeline& operator=(Pipeline&& other) noexcept;       

        VkPipeline getPipeline() const noexcept { return m_pipeline; }
        
    private:
        VkDevice m_device{ VK_NULL_HANDLE };
        VkPipeline m_pipeline{ VK_NULL_HANDLE };
    };
}; // namespace SM

#endif // SM_RESOURCES_PIPELINE_H_
