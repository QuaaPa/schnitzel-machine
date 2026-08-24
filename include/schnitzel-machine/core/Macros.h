#include <vulkan/vulkan.h>

// Construct an version number
#define SM_MAKE_VERSION(major, minor, patch)                            \
    ((((uint32_t)(major)) << 22U) | (((uint32_t)(minor)) << 12U) | ((uint32_t)(patch)))

#define SM_NODISCARD [[nodiscard]]
