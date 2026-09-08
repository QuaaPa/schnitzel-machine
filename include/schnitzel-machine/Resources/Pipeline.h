#ifndef SM_RESOURCES_PIPELINE_H_
#define SM_RESOURCES_PIPELINE_H_

#include <vector>

#include <vulkan/vulkan_core.h>

namespace SM {
    enum class PipelineType {
        GRAPHICS,
        COMPUTE,
        RAY_TRACING
    };
    
    struct GraphicsPipelineDescription {
        std::vector<VkPipelineShaderStageCreateInfo> stages;
        VkPipelineVertexInputStateCreateInfo vertexInput;
        VkPipelineInputAssemblyStateCreateInfo inputAssembly;
        VkPipelineRasterizationStateCreateInfo rasterization;
        VkPipelineMultisampleStateCreateInfo multisample;
        VkPipelineDepthStencilStateCreateInfo depthStencil;
        VkPipelineColorBlendStateCreateInfo colorBlend;
        VkPipelineLayout layout;

        std::vector<VkFormat> colorAttachmentFormats;
        VkFormat depthAttachmentFormat = VK_FORMAT_UNDEFINED;
        VkFormat stencilAttachmentFormat = VK_FORMAT_UNDEFINED;
    };

    class Pipeline {
    public:
        Pipeline(const VkDevice &device, PipelineType type, const GraphicsPipelineDescription& desc);
                
    private:
        void initializeAsGraphicsPipeline(const VkDevice &device, const GraphicsPipelineDescription& desc);
        void initializeAsComputePipeline(const VkDevice &device, const GraphicsPipelineDescription& desc);
        void initializeAsRayTracingPipeline(const VkDevice &device, const GraphicsPipelineDescription& desc);
        
    private:
        VkPipeline m_pipeline{ VK_NULL_HANDLE };
        VkPipelineLayout m_layout{ VK_NULL_HANDLE };
    };
}; // namespace SM

#endif // SM_RESOURCES_PIPELINE_H_
