#ifndef SM_RHI_VULKANRHI_H_
#define SM_RHI_VULKANRHI_H_

#include <vector>
#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>

#include "RHI/Surface.h"
#include "core/Macros.h"
#include "RHI/Instance.h"
#include "RHI/Adapter.h"
#include "RHI/Device.h"
#include "RHI/Queue.h"

namespace SM {    
    class VulkanRHI {
    public:
        VulkanRHI() { };
        ~VulkanRHI() { };
    public:        
        VkInstance createInstance(const SM::InstanceOptions& options);
        // TODO: create obtaining different types of windows
        VkSurfaceKHR createSurface(const SM::WindowHandle& window);        
        VkDevice createDevice(const SM::DeviceOptions &options);

        std::vector<SM::Queue> queryQueues() { return m_queues; };
        
        SM::SMResult destroy();
        
    private:
        SM::Adapter selectAdapter();
        std::vector<QueueDescription> getQueues(const std::vector<QueueRequest> &queueRequests, std::vector<AdapterQueueType> queueTypes);
        
    private:
        SM::Instance m_instance;
        SM::Surface m_surface;
        SM::Adapter m_adapter;   // is a representation of a physical hardware device
        SM::Device m_device;     // represents a logical GPU device
        std::vector<SM::Queue> m_queues;
    };
}
#endif // SM_RHI_VULKANRHI_H_
