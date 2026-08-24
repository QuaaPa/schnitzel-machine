#ifndef SM_CORE_HANDLE_H_
#define SM_CORE_HANDLE_H_

#include <cassert>
#include <cstdint>
#include <type_traits>

#include "core/HashUtils.h"

namespace SM {
    
template<typename T>
class Handle
{
public:
    Handle()
        : m_index(0)
        , m_generation(0)
    {
    }

    bool isValid() const noexcept { return m_generation != 0; }

    uint32_t index() const noexcept { return m_index; }
    uint32_t generation() const noexcept { return m_generation; }

protected:
    explicit Handle(uint32_t index, uint32_t generation)
        : m_index(index)
        , m_generation(generation)
    {
    }

private:
    uint32_t m_index;
    uint32_t m_generation;

    template<typename U>
    friend class Pool;
};

template<typename T>
bool operator==(const SM::Handle<T> &lhs, const SM::Handle<T> &rhs)
{
    return lhs.index() == rhs.index() && lhs.generation() == rhs.generation();
}

template<typename T>
bool operator!=(const SM::Handle<T> &lhs, const SM::Handle<T> &rhs)
{
    return !(lhs == rhs);
}

template<typename T>
bool operator<(const SM::Handle<T> &lhs, const SM::Handle<T> &rhs)
{
    return lhs.index() < rhs.index();
}

// A handle that MUST be valid (wrapper around Handle)
template<typename T>
class RequiredHandle : public Handle<T>
{
public:
#ifdef SM_DEBUG
    RequiredHandle() = delete;
#endif
    using Handle<T>::Handle;

    RequiredHandle(const Handle<T> &handle)
        : Handle<T>(handle.index(), handle.generation())
    {
#ifdef SM_DEBUG
        assert(this->isValid());
#endif
    }

    template<typename U, typename = std::enable_if_t<!std::is_same_v<U, Handle<T>> && std::is_convertible_v<U, Handle<T>>>>
    RequiredHandle(const U &obj)
        : Handle<T>(Handle<T>(obj))
    {
#ifdef SM_DEBUG
        assert(this->isValid());
#endif
    }
};
    
// Alias for raw Handle    
template<typename T>
using OptionalHandle = Handle<T>;

} // namespace SM

// REALIZE HASHING FOR CUSTOM TYPE: SM::HANDLE 
template<typename T>
struct std::hash<SM::Handle<T>> {
    std::size_t operator()(const SM::Handle<T> &handle) const
    {
        uint64_t hash = 0;

        SM::hash_combine(hash, handle.index());
        SM::hash_combine(hash, handle.generation());

        return hash;
    }
};

template<typename T>
struct std::hash<SM::RequiredHandle<T>> {
    std::size_t operator()(const SM::RequiredHandle<T> &handle) const
    {
        uint64_t hash = 0;

        SM::hash_combine(hash, handle.index());
        SM::hash_combine(hash, handle.generation());

        return hash;
    }
};

#endif // SM_CORE_HANDLE_H_
