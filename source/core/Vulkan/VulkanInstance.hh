#ifndef VULKAN_INSTANCE_HH
#define VULKAN_INSTANCE_HH

#include <vulkan/vulkan_core.h>
#include <iostream>
#include <vector>
#include <cstring>
#include "../../utils/DebugUtils.hh"

namespace CORE {
    class VulkanInstance {

    private:
        VkInstance m_instance = VK_NULL_HANDLE;

        std::vector<const char*> m_requiredExtensions = {};
	
        std::vector<const char*> getRequiredExtensions();

    public:
        VulkanInstance();

        void init(const char* appName, uint32_t major, uint32_t minor, uint32_t patch);
        VkInstance getInstance() const;    
        void destroy();

    private:
#ifndef NDEBUG
        bool checkValidationLayerSupport();
	
        const std::vector<const char*> m_validationLayers = {"VK_LAYER_KHRONOS_validation"};
#endif // NDEBUG
    
    };
}

#endif // VULKAN_INSTANCE_HH
