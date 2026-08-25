#ifndef SM_RHI_ADAPTER_H_
#define SM_RHI_ADAPTER_H_

#include <vector>
#include <string>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "RHI/AdapterQueueType.h"
#include "RHI/AdapterSwapchainProperties.h"
#include "core/Macros.h"
#include "RHI/AdapterFeatures.h"

namespace SM {    
    struct AdapterOptions {
        std::vector<std::string> layers; //!< Device-level validation/debug layers
        std::vector<std::string> extensions; //!< Device extensions (e.g., "VK_KHR_swapchain", "VK_KHR_ray_tracing_pipeline")
    };

    struct Adapter {
    public:
        Adapter() { };
        
        Adapter(const Adapter& other) = default;
        Adapter& operator=(const Adapter& other) = default;
        
        SM_NODISCARD std::vector<VkExtensionProperties> extensions() const;        
        SM_NODISCARD VkPhysicalDeviceProperties properties() const;        
        SM_NODISCARD SM::AdapterFeatures features();
        SM_NODISCARD std::vector<SM::AdapterQueueType> queryQueueFamily();       
        SM::AdapterSwapchainProperties querySwapchainProperties(const VkSurfaceKHR &surfaceHandle);
        bool supportsPresentation(const VkSurfaceKHR surfaceHandle, uint32_t queueTypeIndex) const;
        
        void setHandle(VkPhysicalDevice physicalDeviceHandle) noexcept { m_handle = std::move(physicalDeviceHandle); }        
        VkPhysicalDevice getHandle() const noexcept { return m_handle; };
        
    public:
        VkBool32 supportsSynchronization2 { false };
        
    private:
        VkPhysicalDevice m_handle { VK_NULL_HANDLE };
        std::vector<SM::AdapterQueueType> m_queueFamilies;
    };
}
#endif // SM_RHI_ADAPTER_H_
