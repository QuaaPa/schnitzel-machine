#ifndef VULKAN_CONTEXT
#define VULKAN_CONTEXT

#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <cstdint>

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
    VkDevice m_device = VK_NULL_HANDLE;
    VkQueue m_graphicsQueue = VK_NULL_HANDLE;
    VkSurfaceKHR m_surface;
    
    const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;
        bool isComplete() {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };
    
public:
    void createInstance();
    void setupDebugMessenger();
    void pickPhysicalDevice();
    void createLogicalDevice();
    void createSurface(GLFWwindow* pwindow);
    
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
