#ifndef INSTANCE_BUILDER_HH
#define INSTANCE_BUILDER_HH

#include <vulkan/vulkan.h>
#include <vector>

#include "utils/DebugUtils.hh"

struct InstanceBuilder {
    const char *appName = "appName";
    bool validation = false;

    VkInstance build() const;
};

#endif // INSTANCE_BUILDER_HH
