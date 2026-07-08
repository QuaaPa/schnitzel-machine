#ifndef SM_UTILS_DEBUGUTILS_H_
#define SM_UTILS_DEBUGUTILS_H_

#include <iostream>

#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>

namespace sm {
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
                                                        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                        VkDebugUtilsMessageSeverityFlagsEXT messageType,
                                                        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                                        void* pUserData) {

        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
        // TODO: create diffrent response for each severity of the message
        // 
        // -> VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
        // Diagnostic message
        // 
        // -> VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
        // Informational message like the creation of a resource
        // 
        // -> VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
        // Message about behavior that is not necessarily an error,
        // but very likely a bug in your application
        // 
        // -> VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
        // Message about behavior that is invalid and may cause crashes

        // TODO: create diffrent response for each "messageType"
        //
        // -> VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
        // Some event has happened that is unrelated to the specification or performance
        //
        // -> VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
        // Something has happened that violates the specification
        // or indicates a possible mistake
        // 
        // -> VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT
        // Potential non-optimal use of Vulkan
            
        return VK_FALSE;
    }
    
    static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
        createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity =
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;
    }

}

#endif // SM_UTILS_DEBUGUTILS_H_
