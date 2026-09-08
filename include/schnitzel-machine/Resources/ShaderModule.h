#ifndef SM_RESOURCES_SHADERMODULE_H_
#define SM_RESOURCES_SHADERMODULE_H_

#include <vector>
#include <vulkan/vulkan_core.h>

namespace SM {
    class ShaderModule {
    public:
        ShaderModule(VkDevice device, const std::vector<uint32_t> &spirv, VkShaderStageFlagBits stage);

        VkShaderModule get() const noexcept { return m_shaderModule; }
        // TODO
        //
        // destroy
        
    private:
        VkShaderModule m_shaderModule;
    };
}; // namespace SM

#endif // SM_RESOURCES_SHADERMODULE_H_
