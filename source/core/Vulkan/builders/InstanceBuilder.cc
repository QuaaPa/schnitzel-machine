#include "core/Vulkan/builders/InstanceBuilder.h"
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <vulkan/vulkan_core.h>

VkInstance InstanceBuilder::build() const {
        VkApplicationInfo applicationInfo {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO, .pNext = nullptr,
        .pApplicationName = "schnitzel",
        .applicationVersion = VK_MAKE_VERSION(0, 0, 1),
        .pEngineName = "schnitzel-machine",
        .engineVersion = VK_MAKE_VERSION(0, 0, 1),
        .apiVersion = VK_API_VERSION_1_0
    };
  
    uint32_t glfwExtensionCount = 0;
    const char **glfwExtension = nullptr;
    glfwExtension = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    VkInstanceCreateInfo instanceCreateInfo{
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = nullptr,
        .pApplicationInfo = &applicationInfo,
        .enabledLayerCount = 0,
        .enabledExtensionCount = glfwExtensionCount,
        .ppEnabledExtensionNames = glfwExtension,
    };

    const char* validationLayers = "VK_LAYER_KHRONOS_validation";
    if(validation) {
        instanceCreateInfo.enabledLayerCount = 1;
        instanceCreateInfo.ppEnabledLayerNames = &validationLayers;
    }
    
    VkInstance instance;
    if (vkCreateInstance(&instanceCreateInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance");
    }
    return instance;
}
