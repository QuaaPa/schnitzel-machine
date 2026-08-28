#include "RHI/Instance.h"

#include <cstdint>
#include <cstdlib>
#include <cstring>

#include <vulkan/vulkan_core.h>

#include "RHI/ExtensionProperties.h"
#include "RHI/VkResultToString.h"
#include "RHI/VulkanConfig.h"
#include "RHI/Adapter.h"
#include "core/Log.h"

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

void SM::Instance::initialize(uint32_t apiVersion, const SM::InstanceOptions& options) {    
    // check for Vulkan API support by system, fall back to extensions if needed
    uint32_t maxApiVersionSupportedBySystem;
    if (vkEnumerateInstanceVersion) // checking if function is exist (1.1+)
        vkEnumerateInstanceVersion(&maxApiVersionSupportedBySystem);

    if (maxApiVersionSupportedBySystem <= VK_API_VERSION_1_2) {
        SM_LOG_CRITICAL("RHI/Instance",
                        "System does not support the minimum required Vulkan API version (1.2+), which is {}.{}.{}, aborting...",
                        VK_VERSION_MAJOR(maxApiVersionSupportedBySystem),
                        VK_VERSION_MINOR(maxApiVersionSupportedBySystem),
                        VK_VERSION_PATCH(maxApiVersionSupportedBySystem));
        abort();
    }
    if(apiVersion > maxApiVersionSupportedBySystem) {
        SM_LOG_CRITICAL("RHI/Instance",
                        "Downgrading requested Vulkan API Version {}.{}.{} because system only supports {}.{}.{}",
                        VK_VERSION_MAJOR(apiVersion), VK_VERSION_MINOR(apiVersion), VK_VERSION_PATCH(apiVersion),
                        VK_VERSION_MAJOR(maxApiVersionSupportedBySystem),
                        VK_VERSION_MINOR(maxApiVersionSupportedBySystem),
                        VK_VERSION_PATCH(maxApiVersionSupportedBySystem));
        apiVersion = maxApiVersionSupportedBySystem;
    }
    
    VkApplicationInfo appInfo = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = nullptr,
        .pApplicationName = options.applicationName,
        .applicationVersion = options.applicationVersion,
        .pEngineName = options.engineName,
        .engineVersion = options.engineVersion,
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

    // Query the available instance layers
    const auto availableLayers = queryAvailableLayers();

    std::vector<const char *> layers;
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
    const auto availableExtensions = queryExtensions();

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
    if(auto result = vkCreateInstance(&createInfo, nullptr, &m_handle); result != VK_SUCCESS) {
        SM_LOG_CRITICAL("RHI", "{}: Failed to create Instance, aborting...", SM::toString(result));
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
}

void SM::Instance::destroy() {
    if (m_handle != VK_NULL_HANDLE) {
        if (m_debugMessenger != VK_NULL_HANDLE) {
            auto destroyFn = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(m_handle, "vkDestroyDebugUtilsMessengerEXT"));
            if (destroyFn != nullptr) destroyFn(m_handle, m_debugMessenger, nullptr);
        }
        vkDestroyInstance(m_handle, nullptr);

        m_handle = VK_NULL_HANDLE;
        m_debugMessenger = VK_NULL_HANDLE;        
    }    
}

std::vector<SM::Adapter> SM::Instance::queryAdapters() const {
    if(m_handle == VK_NULL_HANDLE) {
        SM_LOG_ERROR("RHI/Instance", "Failed to query adapters, instance is invalid");
        return {};
    }
    
    uint32_t vkPhysicalDeviceCount{ 0 };
    vkEnumeratePhysicalDevices(m_handle, &vkPhysicalDeviceCount, nullptr);
    
    std::vector<VkPhysicalDevice> vkPhysicalDevices;
    vkPhysicalDevices.resize(vkPhysicalDeviceCount);
    if (vkPhysicalDeviceCount != 0) {
        if (auto result = vkEnumeratePhysicalDevices(m_handle, &vkPhysicalDeviceCount, vkPhysicalDevices.data()); result != VK_SUCCESS) {        
            SM_LOG_ERROR("RHI/Instance", "{}: Failed to query adapters", SM::toString(result));
        }
    }

    std::vector<SM::Adapter> adapters;
    adapters.resize(vkPhysicalDeviceCount);
    for (uint32_t i = 0; i < vkPhysicalDeviceCount; ++i) {
        adapters[i].setHandle(vkPhysicalDevices[i]);
    }
    return adapters;
}

std::vector<std::string> SM::Instance::queryAvailableLayers() const {
    uint32_t vkLayerPropertyCount{ 0 };
    vkEnumerateInstanceLayerProperties(&vkLayerPropertyCount, nullptr);

    std::vector<VkLayerProperties> vkLayersProperties;
    vkLayersProperties.resize(vkLayerPropertyCount);
    if(vkLayerPropertyCount != 0) {        
        if (auto result = vkEnumerateInstanceLayerProperties(&vkLayerPropertyCount, vkLayersProperties.data()); result != VK_SUCCESS) {
            SM_LOG_ERROR("RHI", "{}: Failed to query instance layers", SM::toString(result));
        }
    }

    std::vector<std::string> layers;
    layers.reserve(vkLayerPropertyCount);
    for (const auto &properties : vkLayersProperties) {
        layers.push_back(properties.layerName);
    }

    return layers;
}

std::vector<SM::ExtensionProperties> SM::Instance::queryExtensions() const {    
    uint32_t vkExtensionPropertyCount{ 0 };
    vkEnumerateInstanceExtensionProperties(nullptr, &vkExtensionPropertyCount, nullptr);
    
    std::vector<VkExtensionProperties> vkExtensionProperties;
    vkExtensionProperties.resize(vkExtensionPropertyCount);
    if(vkExtensionPropertyCount != 0) {        
        if (vkEnumerateInstanceExtensionProperties(nullptr, &vkExtensionPropertyCount, vkExtensionProperties.data()) != VK_SUCCESS) {
            SM_LOG_ERROR("RHI/Instance", "Failed to query instance extensions");
            return {};
        }
    }

    std::vector<SM::ExtensionProperties> extensionProperties;
    extensionProperties.reserve(vkExtensionPropertyCount);
    for (uint32_t i = 0; i < vkExtensionPropertyCount; ++i) {
        const auto &prop = vkExtensionProperties[i];
        SM::ExtensionProperties extProp{ };
        std::strncpy(extProp.extensionName, vkExtensionProperties[i].extensionName, VK_MAX_EXTENSION_NAME_SIZE); // copying extension name 
        extProp.specVersion = vkExtensionProperties[i].specVersion;
        extensionProperties.emplace_back(extProp);
    }
   
    return extensionProperties;
}
