#ifndef VULKAN_UTILS_HH
#define VULKAN_UTILS_HH

#include <vulkan/vulkan.h>
#include <optional>
#include <vector>

namespace CORE::UTILS {
 
    inline std::optional<uint32_t> findQueueFamilyIndex(VkQueueFlagBits expectedQueueFlag, VkPhysicalDevice device) {
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());    
        int i = 0;
        for (const auto &queueFamily : queueFamilies) {
            if (queueFamily.queueFlags & expectedQueueFlag) {             
                return i;
            }
            i++;
        }
        return std::nullopt;
    }
} // namespace CORE::UTILS
#endif // VULKAN_UTILS_HH
