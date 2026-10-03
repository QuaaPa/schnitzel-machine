#ifndef SM_RHI_VULKANRHI_H_
#define SM_RHI_VULKANRHI_H_

#include <memory>
#include <vector>

#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>

#include "core/TypesDefs.h"
#include "RHI/Instance.h"
#include "RHI/Adapter.h"
#include "RHI/Surface.h"
#include "core/Macros.h"
#include "RHI/Device.h"

namespace SM {
    static VkFence tryCreateFence(VkDevice vkDevice, VkFenceCreateFlags flags = 0) {
        VkFence vkFence = VK_NULL_HANDLE;
        VkFenceCreateInfo vkFenceInfo{};
        vkFenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        vkFenceInfo.pNext = nullptr;
        vkFenceInfo.flags = flags;

        if (auto result = vkCreateFence(vkDevice, &vkFenceInfo, nullptr, &vkFence); result != VK_SUCCESS) {
            SM_LOG_WARN("RHI", "{}: Failed to create fence", SM::toString(result));
        }
        return vkFence;
    }

    static VkSemaphore tryCreateSemaphore(VkDevice vkDevice) {
        VkSemaphore vkSemaphore = VK_NULL_HANDLE;
        VkSemaphoreCreateInfo vkSemaphoreInfo{};
        vkSemaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        vkSemaphoreInfo.pNext = nullptr;
        vkSemaphoreInfo.flags = 0; // must be 0

        if (auto result = vkCreateSemaphore(vkDevice, &vkSemaphoreInfo, nullptr, &vkSemaphore); result != VK_SUCCESS) {
            SM_LOG_WARN("RHI", "{}: Failed to create semaphore", SM::toString(result));
        }

        return vkSemaphore;
    }

    class VulkanRHI {
    public:
        explicit VulkanRHI(uint32_t apiVersion, SM::WindowHandle windowHandle);               
        ~VulkanRHI();
        
        VulkanRHI(const VulkanRHI&)                = delete;
        VulkanRHI& operator=(const VulkanRHI&)     = delete;
        VulkanRHI(VulkanRHI&&) noexcept            = delete;
        VulkanRHI& operator=(VulkanRHI&&) noexcept = delete;

        SM::Device*   device()   const noexcept { return m_device.get(); }
        SM::Adapter*  adapter()  const noexcept { return m_adapter.get(); }
        SM::Surface*  surface()  const noexcept { return m_surface.get(); }
        SM::Instance* instance() const noexcept { return m_instance.get(); }        
        
        void destroy();
        
    private:
        SM::Adapter selectSuitableAdapter(std::vector<SM::Adapter> adapters, const SM::Surface* pSurface) const;

    private:
        std::unique_ptr<SM::Instance> m_instance;
        std::unique_ptr<SM::Surface>  m_surface;
        std::unique_ptr<SM::Adapter>  m_adapter;  // represents a physical device 
        std::unique_ptr<SM::Device>   m_device;
    };
} // namespace SM
#endif // SM_RHI_VULKANRHI_H_
