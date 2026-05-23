#ifndef VULKAN_DEVICE_HH
#define VULKAN_DEVICE_HH

#include <vulkan/vulkan_core.h>
#include "VulkanInstance.hh"
#include "VulkanPhysicalDevice.hh"

#include "../../utils/VulkanUtils.hh"


namespace CORE {
    class VulkanInstance;
    class Device {
    private:
        PhysicalDevice* m_physicalDevice = nullptr;;
        VkDevice m_device;
        VkQueue m_graphicsQueue;
    public:
      void init(VulkanInstance &instance);

      void destroy();

    private:
        const std::vector<const char*> m_validationLayers = {"VK_LAYER_KHRONOS_validation"};
    };
}

#endif // VULKAN_LOGICAL_DEVICE_HH
