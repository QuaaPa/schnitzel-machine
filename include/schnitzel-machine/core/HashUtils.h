#ifndef SM_CORE_HASHCOMBINE_H_
#define SM_CORE_HASHCOMBINE_H_

#include <cstddef>
#include <cstdint>

namespace SM {
    inline void hash_combine(std::size_t& seed, std::size_t value) {
        seed ^= value + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2);
    }

} // namespace SM

#endif // SM_CORE_HASHCOMBINE_H_
