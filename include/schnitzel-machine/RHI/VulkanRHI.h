#ifndef SM_RHI_VULKANRHI_H_
#define SM_RHI_VULKANRHI_H_

#include <vector>
#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>

#include "RHI/Surface.h"
#include "RHI/Swapchain.h"
#include "core/Macros.h"
#include "RHI/Instance.h"
#include "RHI/Adapter.h"
#include "RHI/Device.h"
#include "RHI/Queue.h"

namespace SM {    
    class VulkanRHI {
    public:        
        VkInstance createInstance(const SM::InstanceOptions& options);
        // TODO: create obtaining different types of windows
        VkSurfaceKHR createSurface(const SM::WindowHandle& window);        
        VkPhysicalDevice selectAdapter();
        VkDevice createDevice(const SM::DeviceOptions &options);
        VkSwapchainKHR createSwapchain(const SM::SwapchainOptions& options);

        std::vector<SM::Queue> queryQueues() { return m_queues; };
        
        void destroy();
        
    private:
        std::vector<QueueDescription> getQueues(const std::vector<QueueRequest> &queueRequests, std::vector<SM::QueueFamilyProperties> queueTypes);
        
    private:
        SM::Swapchain m_swapchain;
        SM::Device m_device;     // represents a logical GPU device
        SM::Adapter m_adapter;   // is a representation of a physical hardware device
        std::vector<SM::Queue> m_queues;
        SM::Surface m_surface;
        SM::Instance m_instance;
    };
}
#endif // SM_RHI_VULKANRHI_H_
