#ifndef SM_RESOURCES_PIPELINE_H_
#define SM_RESOURCES_PIPELINE_H_

#include <vector>

#include <vulkan/vulkan_core.h>

#include "RHI/VkResultToString.h"

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
       
        Pipeline(const Pipeline&) = delete;
        Pipeline& operator=(const Pipeline&) = delete;
        Pipeline(Pipeline&& other) noexcept;
        Pipeline& operator=(Pipeline&& other) noexcept;

        SM::Result initializePipelineAsGraphics(VkDevice vkDevice, const GraphicsPipelineDescription& desc);
        SM::Result initializePipelineAsCompute(VkDevice vkDevice, const ComputePipelineDescription& desc);
        SM::Result initializePipelineAsRayTracing(VkDevice vkDevice, const RayTracingPipelineDescription& desc);
        VkPipeline getPipeline() const noexcept { return m_pipeline; }
        
    private:
        VkDevice m_device{ VK_NULL_HANDLE };
        VkPipeline m_pipeline{ VK_NULL_HANDLE };
    };
}; // namespace SM

#endif // SM_RESOURCES_PIPELINE_H_
