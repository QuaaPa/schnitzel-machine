#ifndef QUEUE_FAMILY_INDICIES_HH
#define QUEUE_FAMILY_INDICIES_HH

#include <optional>
#include <cstdint>

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() const {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
}; 

#endif // QUEUE_FAMILY_INDICIES_HH
