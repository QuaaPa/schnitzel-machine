#ifndef SM_RHI_INSTANCE_H_
#define SM_RHI_INSTANCE_H_

#include <vector>
#include <string>

#include <vulkan/vulkan_core.h>

#include "RHI/Adapter.h"
#include "core/Macros.h"
#include "core/TypesDefs.h"

namespace SM {
    struct InstanceOptions {
        //!< Application name (used for Vulkan debugging)
        std::string applicationName{ "UndefinedAppName" };
        //!< Application version
        uint32_t applicationVersion{ SM_MAKE_VERSION(0, 0, 1) };
        //!< Engine version
        uint32_t engineVersion{ SM_MAKE_VERSION(0, 0, 1) };
        //!< Vulkan validation/debug layers to enable (e.g., "VK_LAYER_KHRONOS_validation")
        std::vector<std::string> layers;
        //!< Instance extensions to enable (e.g., "VK_KHR_surface")
        std::vector<std::string> extensions;
    };
    
    class Instance {
    public:
        Instance() { };
        ~Instance();
        
    public:
        SM::SMResult initialize(const SM::InstanceOptions& options);

        SM_NODISCARD std::vector<SM::Adapter> queryAdapters() const;            
        SM_NODISCARD std::vector<std::string> getAvailableInstanceLayers() const;        
        SM_NODISCARD std::vector<VkExtensionProperties>queryInstanceExtensions() const;

        SM_NODISCARD VkInstance& getHandle() { return m_handle; };

        SM::SMResult destroy();
    private:        
        VkInstance m_handle{ VK_NULL_HANDLE };
        VkDebugUtilsMessengerEXT m_debugMessenger{ VK_NULL_HANDLE };
    };    
}; // namespace SM

#endif // SM_RHI_INSTANCE_H_
