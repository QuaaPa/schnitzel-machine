#include "RHI/Instance.h"

#include <cstring>

#include <vulkan/vulkan_core.h>

#include "core/Log.h"
#include "RHI/Adapter.h"
#include "RHI/VulkanConfig.h"
#include "core/TypesDefs.h"

#ifdef SM_BUILD_DEBUG_MODE
VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
                                             VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                             VkDebugUtilsMessageTypeFlagsEXT messageType,
                                             const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                             void *pUserData)
{

    std::string_view cat;
    switch (messageType) {
    case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
        // Some event has happened that is unrelated to the specification or performance
        cat = "GENERAL";
        break;
    case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
        // Something has happened that violates the specification
        // or indicates a possible mistake
        cat = "VALIDATION";
        break;
    case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
        // Potential non-optimal use of Vulkan
        cat = "PERFORMANCE";
        break;
    default:
        cat = "UNKNOWN";
        break;
    }

    
    switch (messageSeverity) {
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
        // Diagnostic message
        SM_LOG_DEBUG(cat, "validation layer: {}", pCallbackData->pMessage);
        break; 
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
        // Informational message like the creation of a resource
        SM_LOG_INFO(cat, "validation layer: {}", pCallbackData->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
        // Message about behavior that is not necessarily an error,
        SM_LOG_WARN(cat, "validation layer: {}", pCallbackData->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
        // Message about behavior that is invalid and may cause crashes
        SM_LOG_ERROR(cat, "validation layer: {}", pCallbackData->pMessage);
        break;
    default:
        SM_LOG_TRACE(cat, "validation layer: {}", pCallbackData->pMessage);
        break;
    }

    return VK_FALSE;
}
#endif // SM_BUILD_DEBUG_MODE

SM::Instance::~Instance() {
    if (m_handle != VK_NULL_HANDLE) {
        SM_LOG_CRITICAL("RHI/Instance", "Instance was not explicitly destroyed, forcing cleanup");
        destroy();
    }
}

SM::SMResult SM::Instance::initialize(const SM::InstanceOptions& options) {
    uint32_t apiVersion;
    if(auto result = vkEnumerateInstanceVersion(&apiVersion); result != VK_SUCCESS) {
        SM_LOG_WARN("RHI", "Failed to enumerate api version, setting default value...");
        apiVersion = VK_API_VERSION_1_2; // default API version        
    }
    
    VkApplicationInfo appInfo = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = nullptr,
        .pApplicationName = options.applicationName.data(),
        .applicationVersion = options.applicationVersion,
        .pEngineName = "Schnitzel-Machine",
        .engineVersion = VK_MAKE_VERSION(0, 1, 0),
        .apiVersion = apiVersion        
    };

    VkInstanceCreateInfo createInfo = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .pApplicationInfo = &appInfo        
    };
    

    // On macOS we need to enable the VK_KHR_PORTABILITY_subset instance extension so that
    // the MoltenVK driver is allowed to be used even though it is technically non-conformant
    // at present. Also see vulkan_config.h. For more detail see the
    // Encountered VK_ERROR_INCOMPATIBLE_DRIVER section of
    // https://vulkan.lunarg.com/doc/sdk/1.3.216.0/mac/getting_started.html
#if defined(__APPLE__)
    createInfo.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif

    // Marging requested and user layers
    std::vector<const char *> requestedLayers = requestedInstanceLayers;
    for (const std::string &userLayer : options.layers) {
        requestedLayers.push_back(userLayer.c_str());
    }

    std::vector<const char *> layers;

    // Query the available instance layers
    const auto availableLayers = getAvailableInstanceLayers();

    for (const char *requestedLayer : requestedLayers) {
        if (std::find(availableLayers.begin(), availableLayers.end(), requestedLayer) != availableLayers.end()) {
            layers.push_back(requestedLayer);
        } else {
            SM_LOG_WARN("RHI", "Unable to find requested layer {}", requestedLayer);
        }
    }
    if (!layers.empty()) {
        createInfo.enabledLayerCount   = static_cast<uint32_t>(layers.size());        
        createInfo.ppEnabledLayerNames = layers.data();
    }    
    
    std::vector<const char *> requestedInstanceExtensions;

    // Query the available instance extensions
    const auto availableExtensions = queryInstanceExtensions();

    const auto defaultRequestedExtensions = SM::getDefaultRequestedInstanceExtensions();
    for (const char *requestedExtension : defaultRequestedExtensions) {
        // Checking if availableExtensions contain requestedExtension
        if (SM::hasExtension(availableExtensions, requestedExtension)) {
            requestedInstanceExtensions.emplace_back(requestedExtension);
        } else {
            SM_LOG_WARN("RHI", "Unable to find default requested instance extension {}", requestedExtension);
        }
    }

    for (const std::string &userExtension : options.extensions) {
        if (SM::hasExtension(availableExtensions, userExtension)) {
            requestedInstanceExtensions.push_back(userExtension.c_str());
        } else {
            SM_LOG_WARN("RHI", "Unable to find user requested instance extensions {}", userExtension);
        }
    }

    if (!requestedInstanceExtensions.empty()) {
        createInfo.enabledExtensionCount   = static_cast<uint32_t>(requestedInstanceExtensions.size());
        createInfo.ppEnabledExtensionNames = requestedInstanceExtensions.data();
    }

#ifdef SM_BUILD_DEBUG_MODE
    const bool hasExtDebugUtilsExt = std::find_if(requestedInstanceExtensions.begin(),
                                                  requestedInstanceExtensions.end(),
                                                  [](const char *name) { return strcmp(name, VK_EXT_DEBUG_UTILS_EXTENSION_NAME) == 0; }) != requestedInstanceExtensions.end();
    VkDebugUtilsMessengerCreateInfoEXT debugUtilsCreateInfo{};
    if (hasExtDebugUtilsExt) {
        SM_LOG_DEBUG("RHI", "Registering Validation Debug Callback");
        // Provide the debug utils creation info to the instance creation info so it can be used during instance creation
        debugUtilsCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debugUtilsCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        debugUtilsCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        debugUtilsCreateInfo.pfnUserCallback = debugCallback;
        debugUtilsCreateInfo.pUserData = nullptr; // Optional

        createInfo.pNext = &debugUtilsCreateInfo;        
    }
#endif // SM_BUILD_DEBUG_MODE
    
    // Try to create the instance    
    if(vkCreateInstance(&createInfo, nullptr, &m_handle) != VK_SUCCESS) {
        SM_LOG_CRITICAL("RHI", "Failed to create Instance, aborting...");
        return SM_FAILURE;
    }

#ifdef SM_BUILD_DEBUG_MODE
    // Now create the debug utils logger for ourselves (using the same callback as the instance)
    if (hasExtDebugUtilsExt) {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_handle, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr) {
            if (func(m_handle, &debugUtilsCreateInfo, nullptr, &m_debugMessenger) != VK_SUCCESS)
                m_debugMessenger = VK_NULL_HANDLE;
        }
    }
#endif // SM_BUILD_DEBUG_MODE

    return SM_SUCCESS;
}

SM::SMResult SM::Instance::destroy() {
    if (m_handle != VK_NULL_HANDLE) {
        if (m_debugMessenger != VK_NULL_HANDLE) {
            auto destroyFn = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(m_handle, "vkDestroyDebugUtilsMessengerEXT"));
            if (destroyFn != nullptr) destroyFn(m_handle, m_debugMessenger, nullptr);
        }
        vkDestroyInstance(m_handle, nullptr);

        m_handle = VK_NULL_HANDLE;
        m_debugMessenger = VK_NULL_HANDLE;        
    }

    return SM_SUCCESS;
}

std::vector<std::string> SM::Instance::getAvailableInstanceLayers() const {
    uint32_t layerCount{ 0 };
    if (vkEnumerateInstanceLayerProperties(&layerCount, nullptr) != VK_SUCCESS) {
        SM_LOG_ERROR("RHI", "Unable to enumerate instance layers");
        return { };
    }

    std::vector<VkLayerProperties> vkLayers(layerCount);
    if (vkEnumerateInstanceLayerProperties(&layerCount, vkLayers.data()) != VK_SUCCESS) {
        SM_LOG_ERROR("RHI", "Unable to query instance layers");
        return { };
    }

    std::vector<std::string> layers;
    layers.reserve(layerCount);
    for (const auto &properties : vkLayers) {
        layers.push_back(properties.layerName);
    }

    return layers;
}


std::vector<VkExtensionProperties> SM::Instance::queryInstanceExtensions() const {    
    uint32_t extensionCount{ 0 };
    if (vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr) != VK_SUCCESS) {
        SM_LOG_ERROR("RHI/Instance", "Failed to enumerate instance extensions");
        return {};
    };

    std::vector<VkExtensionProperties> extensions(extensionCount);
    if (vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data()) != VK_SUCCESS) {
        SM_LOG_ERROR("RHI/Instance", "Failed to query instance extensions");
        return {};
    }

    return extensions;
}

std::vector<SM::Adapter> SM::Instance::queryAdapters() const {
    if(m_handle == VK_NULL_HANDLE) {
        SM_LOG_ERROR("RHI/Instance", "Failed to query adapters, instance is invalid");
        return {};
    }
    uint32_t physicalDeviceCount{ 0 };
    if (vkEnumeratePhysicalDevices(m_handle, &physicalDeviceCount, nullptr) != VK_SUCCESS) {
        SM_LOG_ERROR("RHI/Instance", "Failed to enumerate adapters");
        return {};
    };

    if (physicalDeviceCount == 0) {
        SM_LOG_CRITICAL("RHI/Instance", "No valid physical devices found");
        return {};
    }
    
    std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);   
    if (vkEnumeratePhysicalDevices(m_handle, &physicalDeviceCount, physicalDevices.data()) != VK_SUCCESS) {        
        SM_LOG_ERROR("RHI/Instance", "Failed to query adapters");
        return {};
    }

    std::vector<SM::Adapter> adapters(physicalDeviceCount);
    for (uint32_t i = 0; i < physicalDeviceCount; ++i) {
        adapters[i].setHandle(physicalDevices[i]);
    }
    return adapters;
}

