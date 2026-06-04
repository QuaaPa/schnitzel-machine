// #include "core/Vulkan/VulkanContext.hh"
// #include <GLFW/glfw3.h>
// #include <vulkan/vulkan_core.h>
// #include <cstdint>
// #include <stdexcept>
// #include <cstring>
// #include <set>

// void VulkanContext::createInstance() {
// }


// std::vector<const char *> VulkanContext::getRequiredExtensions() {
//     uint32_t glfwExtensionCount = 0;
//     const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

//     std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

// #ifndef NDEBUG
//     extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
// #endif
//     return extensions;
// }

// void VulkanContext::setupDebugMessenger() {
// #ifndef NDEBUG
//     VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo{};
//     DBG::populateDebugMessengerCreateInfo(debugUtilsMessengerCreateInfo);
//     if (CreateDebugUtilsMessengerEXT(m_instance, &debugUtilsMessengerCreateInfo, nullptr, &m_debugMessenger) != VK_SUCCESS)  {
//       throw std::runtime_error("failed to set up debug messenger!");
//   }
// #else
//     return;
// #endif // NDEBUG
// }

// VkResult VulkanContext::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
//     auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
//     if (func != nullptr) {
//         return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
//     } else {
//         return VK_ERROR_EXTENSION_NOT_PRESENT;
//     }
// }

// void VulkanContext::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
//     auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
//     if (func != nullptr) {
//         func(instance, debugMessenger, pAllocator);
//     }
// }

// void VulkanContext::destroy() {
//     vkDestroyDevice(m_device, nullptr);
// #ifndef NDEBUG
//     DestroyDebugUtilsMessengerEXT(m_instance, m_debugMessenger, nullptr);
// #endif // NDEBUG
//     vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
//     vkDestroyInstance(m_instance, nullptr);
// }

// void VulkanContext::pickPhysicalDevice() {
//     uint32_t deviceCount = 0;
//     vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);
//     if(deviceCount == 0) {
//         throw std::runtime_error("failed to find GPUs with Vulkan support");
//     }
//     std::vector<VkPhysicalDevice> devices(deviceCount);
//     vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());
//     for(const auto& device : devices) {
//         if(isDeviceSuitable(device)) {
//             m_physcialDevice = device;
//             break;
//         }
//     }
//     if (m_physcialDevice == VK_NULL_HANDLE) {
//         throw std::runtime_error("failed to find a suitable GPU");
//     }
// }

// bool VulkanContext::isDeviceSuitable(VkPhysicalDevice device) {
//     // VkPhysicalDeviceProperties deviceProperties;
//     // vkGetPhysicalDeviceProperties(device, &deviceProperties);
//     // VkPhysicalDeviceFeatures deviceFeatures;
//     // vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

//     // deviceProperties. ...
//     //                   ...

//     QueueFamilyIndices indices = findQueueFamilies(device);

//     return indices.isComplete();
// }

// QueueFamilyIndices VulkanContext::findQueueFamilies(VkPhysicalDevice device) {
//     QueueFamilyIndices indices;

//     uint32_t queueFamilyCount = 0;
//     vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

//     std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
//     vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

//     int i = 0;
//     for (const auto &queueFamily : queueFamilies) {
//         VkBool32 presentSupport = false;
//         vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_surface, &presentSupport);
//         vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
        
//         if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
//           indices.graphicsFamily = i;
//         }
//         if (presentSupport) {
//             indices.presentFamily = i;
//         }
//         if (indices.isComplete()) {
//             break;
//         }
            
//         i++;
//     }
    
//     return indices;
// }

// void VulkanContext::createLogicalDevice() {
//     QueueFamilyIndices indices = findQueueFamilies(m_physcialDevice);
  
//     std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
//     std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(),
//                                               indices.presentFamily.value()};
//     float queuePiority = 1.0f;
//     for(uint32_t queueFamily : uniqueQueueFamilies) {
//         VkDeviceQueueCreateInfo queueCreateInfo{
//             .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
//             .queueFamilyIndex = queueFamily, .queueCount = 1,
//             .pQueuePriorities = &queuePiority
//         };
//         queueCreateInfos.push_back(queueCreateInfo);
//     }
//     VkPhysicalDeviceFeatures deviceFeatures{};
//     VkDeviceCreateInfo createInfo {
//         .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
//         .queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size()),
//         .pQueueCreateInfos = queueCreateInfos.data(),
//         .enabledExtensionCount = 0,
//         .pEnabledFeatures = &deviceFeatures,
//     };
//     if(vkCreateDevice(m_physcialDevice, &createInfo, nullptr, &m_device) != VK_SUCCESS) {
//         throw std::runtime_error("failes to create logical device");
//     }

//     vkGetDeviceQueue(m_device, indices.graphicsFamily.value(), 0, &m_graphicsQueue);
//     vkGetDeviceQueue(m_device, indices.presentFamily.value(), 0, &m_presentQueue);
// }

// void VulkanContext::createSurface(GLFWwindow* pwindow) {
//     if(glfwCreateWindowSurface(m_instance, pwindow, nullptr, &m_surface)) {
//         throw std::runtime_error("failed to create window surface");
//     }
// }
