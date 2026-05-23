#ifndef VULKAN_MANAGER_HH
#define VULKAN_MANAGER_HH

#include "VulkanInstance.hh"
#include "VulkanWindowSurface.hh"
#include "VulkanDevice.hh"
#include <GLFW/glfw3.h>

namespace CORE {
    class VulkanManager {
    private:
        VulkanInstance m_instance;
        WindowSurface m_surface;
        Device m_device;
    public:
        VulkanManager();
        void init(const char* appName, GLFWwindow* window);
        void destroy();
    
        VulkanManager(const VulkanManager &other) = delete;
        VulkanManager(VulkanManager &&other) = delete;
        VulkanManager &operator=(const VulkanManager &other) = delete;
        VulkanManager &operator=(VulkanManager &&other) = delete;

    private:
#ifndef NDEBUG
        VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
                                              const VkAllocationCallbacks *pAllocator,
                                              VkDebugUtilsMessengerEXT *pDebugMessenger);
        void DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                           VkDebugUtilsMessengerEXT debugMessenger,
                                           const VkAllocationCallbacks *pAllocator);
        void setupDebugMessanger();
    
        VkDebugUtilsMessengerCreateInfoEXT m_messengerInfo;
        VkDebugUtilsMessengerEXT m_debugMessanger;
#endif // NDEBUG  
    };
}

#endif // VULKAN_MANAGER_HH
