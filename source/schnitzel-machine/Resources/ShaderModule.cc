#include "Resources/ShaderModule.h"
#include "RHI/VkResultToString.h"
#include "core/Log.h"
#include <cstdint>
#include <vector>
#include <vulkan/vulkan_core.h>

SM::ShaderModule::ShaderModule()
    : m_device(VK_NULL_HANDLE),
      m_shaderModule(VK_NULL_HANDLE)
      
{
    
}

SM::ShaderModule::ShaderModule(SM::ShaderModule&& other) noexcept {
    m_shaderModule = other.m_shaderModule;
    m_device = other.m_device;
    other.m_shaderModule = VK_NULL_HANDLE;
    other.m_device = VK_NULL_HANDLE;
}

SM::ShaderModule& SM::ShaderModule::operator=(SM::ShaderModule&& other) noexcept {
    if (this != &other) {
        destroy();
        m_shaderModule = other.m_shaderModule;
        m_device = other.m_device;
        other.m_shaderModule = VK_NULL_HANDLE;
        other.m_device = VK_NULL_HANDLE;
    }
    return *this;
}

SM::Result SM::ShaderModule::initializeShaderModule(VkDevice vkDevice, const std::vector<uint32_t> &spirv, VkShaderStageFlagBits stage) {
    m_device = vkDevice;
    
    VkShaderModuleCreateInfo vkShaderModuleInfo{};
    vkShaderModuleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    vkShaderModuleInfo.codeSize = spirv.size() * sizeof(uint32_t);
    vkShaderModuleInfo.pCode = spirv.data();

    return vkCreateShaderModule(m_device, &vkShaderModuleInfo, nullptr, &m_shaderModule);
}

void SM::ShaderModule::destroy() {
    if(m_shaderModule != VK_NULL_HANDLE) {
        vkDestroyShaderModule(m_device, m_shaderModule, nullptr);
        m_shaderModule = VK_NULL_HANDLE;
    }
}
