#ifndef SM_RESOURCES_SHADERMODULE_H_
#define SM_RESOURCES_SHADERMODULE_H_

#include <vector>
#include <vulkan/vulkan_core.h>

namespace SM {
    class ShaderModule {
    public:
        ShaderModule() = default;
        ShaderModule(VkDevice device, const std::vector<uint32_t> &spirv, VkShaderStageFlagBits stage);
        ~ShaderModule();

        ShaderModule(const ShaderModule&) = delete;
        ShaderModule& operator=(const ShaderModule&) = delete;
        ShaderModule(ShaderModule&& other) noexcept;
        ShaderModule& operator=(ShaderModule&& other) noexcept;

        VkShaderModule getShaderModule() const noexcept { return m_shaderModule; }
        
    private:
        VkDevice m_device{ VK_NULL_HANDLE };
        VkShaderModule m_shaderModule {VK_NULL_HANDLE };
    };
}; // namespace SM

#endif // SM_RESOURCES_SHADERMODULE_H_
