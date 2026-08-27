#ifndef SM_RHI_INSTANCE_H_
#define SM_RHI_INSTANCE_H_

#include <vector>
#include <string>

#include <vulkan/vulkan_core.h>

#include "VulkanConfig.h"
#include "RHI/Adapter.h"
#include "core/Macros.h"
#include "RHI/ExtensionProperties.h"

namespace SM {
    struct InstanceOptions {
        uint32_t apiVersion{SM_MIN_REQUIRED_VULKAN_API_VERSION};
        const char* applicationName{ "UndefinedAppName" };
        uint32_t applicationVersion{ SM_MAKE_VERSION(0, 0, 1) };
        const char* engineName{ "UndefinedEngineName" };
        uint32_t engineVersion{ SM_MAKE_VERSION(0, 0, 1) };
        
        std::vector<std::string> layers;
        std::vector<std::string> extensions;
    };
    
    class Instance {
    public:
        void initialize(const SM::InstanceOptions& options);

        SM_NODISCARD std::vector<SM::Adapter> queryAdapters() const;            
        SM_NODISCARD std::vector<std::string> queryAvailableLayers() const;        
        SM_NODISCARD std::vector<SM::ExtensionProperties> queryExtensions() const;

        SM_NODISCARD VkInstance getHandle() const noexcept { return m_handle; };

        void destroy();
    private:        
        VkInstance m_handle{ VK_NULL_HANDLE };
        VkDebugUtilsMessengerEXT m_debugMessenger{ VK_NULL_HANDLE };
    };    
}; // namespace SM

#endif // SM_RHI_INSTANCE_H_
