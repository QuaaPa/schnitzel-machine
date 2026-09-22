#ifndef SM_RHI_VULKANRHI_H_
#define SM_RHI_VULKANRHI_H_

#include <vector>

#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>

#include "RHI/VkResultToString.h"
#include "core/Log.h"
#include "core/TypesDefs.h"
#include "RHI/Instance.h"
#include "RHI/Adapter.h"
#include "RHI/Surface.h"
#include "core/Macros.h"
#include "RHI/Device.h"
#include "RHI/Queue.h"

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

    struct RHIOptions {
        uint32_t apiVersion; // Required Vulkan API version
        const SM::WindowHandle& window;

        const SM::InstanceOptions& InstanceOptions;
        const SM::DeviceOptions& DeviceOptions;
    };

    class VulkanRHI {
    public:
        void initialize(const RHIOptions& options);

        std::vector<SM::Queue> queryQueues() { return m_queues; };
        SM::Result deviceWaitIdle();

        SM::Adapter getAdapter() const noexcept { return m_adapter; }
        SM::Device getDevice() const noexcept { return m_device; }
        SM::Surface getSurface() const noexcept { return m_surface; }

        void destroy();
        
    private:
        VkPhysicalDevice selectSuitableAdapter(const std::vector<SM::Adapter> &adapter) const;
        
    private:
        SM::Device m_device;     // represents a logical GPU device
        SM::Adapter m_adapter;   // is a representation of a physical hardware device
        std::vector<SM::Queue> m_queues;
        SM::Surface m_surface;
        SM::Instance m_instance;
    };
} // namespace SM
#endif // SM_RHI_VULKANRHI_H_
