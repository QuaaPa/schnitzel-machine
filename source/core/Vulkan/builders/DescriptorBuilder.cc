#include <cstdint>
#include <stdexcept>

#include <glm/glm.hpp>
#include <vector>
#include <vulkan/vulkan_core.h>

#include "core/Vulkan/builders/DescriptorBuilder.h"
#include "core/Vulkan/VulkanDescriptor.h"
#include "core/Vulkan/VulkanManager.h"

static VkDescriptorPool createDescriptorPool(VkDevice dev) {    
    VkDescriptorPoolSize poolSize {
        .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT)
    };
    
    VkDescriptorPoolCreateInfo poolInfo {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT),
        .poolSizeCount = 1,
        .pPoolSizes = &poolSize
    };

    VkDescriptorPool dscrPool;
    if(vkCreateDescriptorPool(dev, &poolInfo, nullptr, &dscrPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }
    return dscrPool;
}

VulkanDescriptor DescriptorBuilder::build() const {
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
    
    VkDescriptorPool dscrPool = createDescriptorPool(logicalDevice);
    
    std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, dscrSetLayout);
    VkDescriptorSetAllocateInfo allocInfo {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .descriptorPool = dscrPool,
        .descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT),
        .pSetLayouts = layouts.data()
    };

    std::vector<VkDescriptorSet> dscrSets(MAX_FRAMES_IN_FLIGHT);
    if(vkAllocateDescriptorSets(logicalDevice, &allocInfo, dscrSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }
    
    return VulkanDescriptor {
        .descriptorSetLayout = dscrSetLayout,
        .descriptorPool = dscrPool,
        .descriptorSets = dscrSets
    };
}
