#ifndef VULKAN_CONTEXT
#define VULKAN_CONTEXT

#include <GLFW/glfw3.h>
#include <cstdint>
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>
#include <vector>
#include <optional>

#include "utils/DebugUtils.hh"


class VulkanContext {
  // VulkanContext:
  // instance - done
  // ValidationLayers - done 
  // device 
  // physicalDevice
  // queues
private:
    VkInstance m_instance = VK_NULL_HANDLE;
    VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;
    VkPhysicalDevice m_physcialDevice = VK_NULL_HANDLE;
    
    const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        bool isComplete() {
            return graphicsFamily.has_value();
        }
    };
    
public:
    void createInstance();
    void setupDebugMessenger();
    void pickPhysicalDevice();
    
    void destroy();
    
private:
    bool checkValidationLayerSupport();
    std::vector<const char *> getRequiredExtensions();
    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
                                          const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                          const VkAllocationCallbacks *pAllocator,
                                          VkDebugUtilsMessengerEXT *pDebugMessenger);
    void DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                       VkDebugUtilsMessengerEXT debugMessenger,
                                       const VkAllocationCallbacks *pAllocator);
    bool isDeviceSuitable(VkPhysicalDevice device);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
};

#endif // VULKAN_CONTEXT
