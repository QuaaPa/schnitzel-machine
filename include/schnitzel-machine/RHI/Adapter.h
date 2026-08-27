#ifndef SM_RHI_ADAPTER_H_
#define SM_RHI_ADAPTER_H_

#include <vector>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "RHI/ExtensionProperties.h"
#include "RHI/QueueFamilyProperties.h"
#include "RHI/AdapterSwapchainProperties.h"
#include "core/Macros.h"
#include "RHI/AdapterFeatures.h"

namespace SM {    
    // struct AdapterOptions {
    //     std::vector<std::string> layers; //!< Device-level validation/debug layers
    //     std::vector<std::string> extensions; //!< Device extensions (e.g., "VK_KHR_swapchain", "VK_KHR_ray_tracing_pipeline")
    // };

    struct Adapter {
    public:                      
        SM_NODISCARD std::vector<SM::ExtensionProperties> extensions() const;        
        SM_NODISCARD VkPhysicalDeviceProperties properties() const;        
        SM_NODISCARD SM::AdapterFeatures features() const ;
        SM_NODISCARD std::vector<SM::QueueFamilyProperties> queryQueueFamilyProperties() const;       
        SM::AdapterSwapchainProperties querySwapchainProperties(const VkSurfaceKHR &surfaceHandle) const;
        bool supportsPresentation(const VkSurfaceKHR &surfaceHandle, uint32_t queueTypeIndex) const;
        
        void setHandle(VkPhysicalDevice physicalDeviceHandle) noexcept { m_handle = std::move(physicalDeviceHandle); }        
        SM_NODISCARD VkPhysicalDevice getHandle() const noexcept { return m_handle; };
        
    private:                
        VkPhysicalDevice m_handle { VK_NULL_HANDLE };
    };
}
#endif // SM_RHI_ADAPTER_H_
