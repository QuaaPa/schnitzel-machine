#include "Resources/ShaderModule.h"
#include "RHI/VkResultToString.h"
#include "core/Log.h"
#include <cstdint>
#include <vector>

SM::ShaderModule::ShaderModule(VkDevice device, const std::vector<uint32_t> &spirv, VkShaderStageFlagBits stage) {

    VkShaderModuleCreateInfo vkShaderModuleInfo{};
    vkShaderModuleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    vkShaderModuleInfo.codeSize = spirv.size() * sizeof(uint32_t);
    vkShaderModuleInfo.pCode = spirv.data();

    if (auto result = vkCreateShaderModule(device, &vkShaderModuleInfo, nullptr, &m_shaderModule); result != VK_SUCCESS) {
        SM_LOG_ERROR("Res/ShaderModule", "{}: Failed to create shader module", SM::toString(result));
    }
}
