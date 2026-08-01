#include <stdexcept>

#include <glm/glm.hpp>

#include "core/Vulkan/builders/DescriptorSetBuilder.h"
#include "core/Vulkan/VulkanDescriptroSet.h"

VulkanDescriptorSet DescriptorSetBuilder::build() const {
    
    struct UniformBufferObject {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    };
    
    VkDescriptorSetLayoutBinding uboLayoutBinding{
        .binding = 0,
        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
        .pImmutableSamplers = nullptr
    };

    VkDescriptorSetLayoutCreateInfo layoutInfo {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .bindingCount = 1,
        .pBindings = &uboLayoutBinding
    };
    
    VkDescriptorSetLayout dscrSetLayout;
    if(vkCreateDescriptorSetLayout(logicalDevice, &layoutInfo, nullptr, &dscrSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout");
    }

    VkPipelineLayout pipelineLayout;
    VkPipelineLayoutCreateInfo pipelineLayoutInfo {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .setLayoutCount = 1,
        .pSetLayouts = &dscrSetLayout
    };

    
    return VulkanDescriptorSet {
        .descriptorSetLayout = dscrSetLayout,
        .pipelineLayout = pipelineLayout
    };
}
