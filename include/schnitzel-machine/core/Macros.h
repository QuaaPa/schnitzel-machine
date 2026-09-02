#include <vulkan/vulkan.h>

// Construct an version number
#define SM_MAKE_VERSION(major, minor, patch)                            \
    ((((uint32_t)(major)) << 22U) | (((uint32_t)(minor)) << 12U) | ((uint32_t)(patch)))

#define SM_NODISCARD [[nodiscard]]

// SM_CLAMP:
// Reference to lo if v is less than lo,
// reference to hi if hi is less than v,
// otherwise reference to v. 
#define SM_CLAMP(v, lo, hi) ((v) < (lo) ? (lo) : ((hi) < (v) ? (hi) : (v)))
