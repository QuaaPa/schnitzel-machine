#ifndef SM_RHI_ADAPTER_H_
#define SM_RHI_ADAPTER_H_

#include <vector>

#include <vulkan/vulkan_core.h>

#include "RHI/AdapterFeatures.h"
#include "RHI/AdapterSwapchainProperties.h"
#include "RHI/ExtensionProperties.h"
#include "RHI/QueueFamilyProperties.h"
#include "core/Macros.h"

namespace SM {
    struct Adapter {
    public:
        Adapter(VkPhysicalDevice vkPhysicalDevice);

        Adapter(const Adapter& other);
        Adapter& operator=(const Adapter& other);
        Adapter(Adapter&& other) noexcept;
        Adapter& operator=(Adapter&& other) noexcept;

        SM_NODISCARD std::vector<SM::ExtensionProperties> extensions() const;
        SM_NODISCARD VkPhysicalDeviceProperties properties() const;
        SM_NODISCARD SM::AdapterFeatures features() const;
        SM_NODISCARD std::vector<SM::QueueFamilyProperties> queryQueueFamilyProperties() const;
        SM_NODISCARD SM::AdapterSwapchainProperties querySwapchainProperties(VkSurfaceKHR vkSurface) const;

        SM_NODISCARD VkPhysicalDevice getHandle() const noexcept { return m_handle; };

    private:
        VkPhysicalDevice m_handle{ VK_NULL_HANDLE };
    };
} // namespace SM
#endif // SM_RHI_ADAPTER_H_
