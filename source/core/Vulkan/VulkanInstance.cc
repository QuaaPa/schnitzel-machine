#include "VulkanInstance.hh"
#include <GLFW/glfw3.h>
#include <cstdint>
#include <stdexcept>
#include <vulkan/vulkan_core.h>
#include <vector>

CORE::VulkanInstance::VulkanInstance() {
    /* DO NOTHING */
}

void CORE::VulkanInstance::init(const char* appName, uint32_t major, uint32_t minor, uint32_t patch) {
#ifndef NDEBUG
    if (!checkValidationLayerSupport()) {
        throw std::runtime_error("validation layers requested, but not available!");
    }  
#endif // NDEBUG  
    VkApplicationInfo appInfo{
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO, .pNext = nullptr,
        .pApplicationName = appName,
        .applicationVersion = VK_MAKE_VERSION(major, minor, patch),
        .pEngineName = "Schnitzel Engine",
        .engineVersion = VK_MAKE_VERSION(0, 1, 0),
        .apiVersion = VK_API_VERSION_1_0
    };

    VkInstanceCreateInfo instanceInfo{
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = nullptr,
        .pApplicationInfo = &appInfo,
    };

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
#ifndef NDEBUG
    instanceInfo.enabledLayerCount = static_cast<uint32_t>(m_validationLayers.size());
    instanceInfo.ppEnabledLayerNames = m_validationLayers.data();
    dbg::populateDebugMessengerCreateInfo(debugCreateInfo);
    instanceInfo.pNext = nullptr;
#else
    instanceInfo.enabledLayerCount = 0;
#endif // NDEBUG
    m_requiredExtensions = getRequiredExtensions();
    instanceInfo.enabledExtensionCount = static_cast<uint32_t>(m_requiredExtensions.size());
    instanceInfo.ppEnabledExtensionNames = m_requiredExtensions.data();    
    
    if (vkCreateInstance(&instanceInfo, nullptr, &m_instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }      
}

VkInstance CORE::VulkanInstance::getInstance() const {
    return m_instance; 
}

void CORE::VulkanInstance::destroy() {
    if(m_instance != VK_NULL_HANDLE) {
        vkDestroyInstance(m_instance, nullptr);
        m_instance = VK_NULL_HANDLE;
    }
    
}

#ifndef NDEBUG
bool CORE::VulkanInstance::checkValidationLayerSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for(const char* layerName : m_validationLayers) {
        bool layerFound = false;

        for (const auto &layerProperties : availableLayers) {
            if(strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;            
            }
        }
        if (!layerFound) {
            return false;
        }
    }
    
    return true;
}
#endif // NDEBUG

std::vector<const char*> CORE::VulkanInstance::getRequiredExtensions() {
    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char *> extensions(glfwExtensions,
                                         glfwExtensions + glfwExtensionCount);
#ifndef NDEBUG
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    for(auto i : extensions) {
        std::cout<<"RequiredInstanceExtensions:"<<i<<std::endl;
    }
#endif // NDEBUG    
    return extensions;
}

