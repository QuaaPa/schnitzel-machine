#ifndef SM_RHI_VULKANRHI_H_
#define SM_RHI_VULKANRHI_H_

#include <vector>

#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>

#include "RHI/VkResultToString.h"
#include "core/TypesDefs.h"
#include "RHI/Swapchain.h"
#include "RHI/Instance.h"
#include "RHI/Adapter.h"
#include "RHI/Surface.h"
#include "core/Macros.h"
#include "RHI/Device.h"
#include "RHI/Queue.h"

namespace SM {

    struct RHIOptions {
        uint32_t                   apiVersion; // Required Vulkan API version 
        const SM::WindowHandle     &window;
        
        const SM::InstanceOptions  &InstanceOptions;
        const SM::DeviceOptions    &DeviceOptions;
        const SM::SwapchainOptions &SwapchainOptions;
    };
    
    class VulkanRHI {
    public:
        void initialize(const RHIOptions &options);
        
        std::vector<SM::Queue> queryQueues() { return m_queues; };
        SM::Result deviceWaitIdle();        
        void destroy();
        
    private:
        std::vector<QueueDescription> getQueues(const std::vector<QueueRequest> &queueRequests, std::vector<SM::QueueFamilyProperties> queueTypes);

        VkPhysicalDevice selectSuitableAdapter(const std::vector<SM::Adapter> &adapter) const;
        
    private:
        std::vector<VkImage> m_swapchainImages;
        SM::Swapchain m_swapchain;
        SM::Device m_device;     // represents a logical GPU device
        SM::Adapter m_adapter;   // is a representation of a physical hardware device
        std::vector<SM::Queue> m_queues;
        SM::Surface m_surface;
        SM::Instance m_instance;
    };
}
#endif // SM_RHI_VULKANRHI_H_
