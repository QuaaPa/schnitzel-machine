#include "Resources/ShaderModule.h"
#include "RHI/VkResultToString.h"
#include "core/Log.h"
#include <cstdint>
#include <vector>
#include <vulkan/vulkan_core.h>

SM::ShaderModule::ShaderModule(VkDevice device, const std::vector<uint32_t> &spirv, VkShaderStageFlagBits stage)
    :m_device(device)
{
    VkShaderModuleCreateInfo vkShaderModuleInfo{};
    vkShaderModuleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    vkShaderModuleInfo.codeSize = spirv.size() * sizeof(uint32_t);
    vkShaderModuleInfo.pCode = spirv.data();

    if (auto result = vkCreateShaderModule(device, &vkShaderModuleInfo, nullptr, &m_shaderModule); result != VK_SUCCESS) {
        SM_LOG_ERROR("Res/ShaderModule", "{}: Failed to create shader module", SM::toString(result));
    }
}

SM::ShaderModule::~ShaderModule() {
    if(m_shaderModule != VK_NULL_HANDLE) {
        vkDestroyShaderModule(m_device, m_shaderModule, nullptr);
    }
}

SM::ShaderModule::ShaderModule(SM::ShaderModule&& other) noexcept {
    m_shaderModule = other.m_shaderModule;
    m_device = other.m_device;
    other.m_shaderModule = VK_NULL_HANDLE;
    other.m_device = VK_NULL_HANDLE;
}

SM::ShaderModule& SM::ShaderModule::operator=(SM::ShaderModule&& other) noexcept {
    if (this != &other) {
        if (m_shaderModule != VK_NULL_HANDLE) {
            vkDestroyShaderModule(m_device, m_shaderModule, nullptr);
        }
        m_shaderModule = other.m_shaderModule;
        m_device = other.m_device;
        other.m_shaderModule = VK_NULL_HANDLE;
        other.m_device = VK_NULL_HANDLE;
    }
    return *this;
}
