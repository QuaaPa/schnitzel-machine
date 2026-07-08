#ifndef SM_CORE_VULKAN_BUILDERS_INSTANCEBUILDER_H_
#define SM_CORE_VULKAN_BUILDERS_INSTANCEBUILDER_H_

#include <vulkan/vulkan.h>

struct InstanceBuilder {
    const char *appName = "appName";
    bool validation = false;

    VkInstance build() const;
};

#endif // SM_CORE_VULKAN_BUILDERS_INSTANCEBUILDER_H_
