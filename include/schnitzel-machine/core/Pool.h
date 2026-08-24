#ifndef SM_CORE_POOL_H_
#define SM_CORE_POOL_H_

#include <vector>
#include <cstdint>
#include <cassert>
#include <utility>

#include "Handle.h"

namespace SM {

    template<typename T>
    class Pool
    {
    public:
        Pool() = default;

        // Reserves memory for N slots in advance to avoid reallocations at runtime
        explicit Pool(size_t reserveCount) {
            m_slots.reserve(reserveCount);
        }

        Handle<T> insert(T value)
        {
            if (!m_freeList.empty()) {
                uint32_t idx = m_freeList.back();
                m_freeList.pop_back();

                Slot &slot = m_slots[idx];
                slot.value = std::move(value);
                slot.alive = true;
                return Handle<T>(idx, slot.generation);
            }

            m_slots.push_back(Slot{std::move(value), /*generation=*/1, /*alive=*/true});
            return Handle<T>(static_cast<uint32_t>(m_slots.size() - 1), 1);
        }

        T* get(OptionalHandle<T> handle) noexcept
        {
            if (!isValidSlot(handle)) return nullptr;
            return &m_slots[handle.index()].value;
        }

        const T* get(OptionalHandle<T> handle) const noexcept
        {
            if (!isValidSlot(handle)) return nullptr;
            return &m_slots[handle.index()].value;
        }

        T& getRequired(RequiredHandle<T> handle) noexcept
        {
            assert(isValidSlot(handle) && "Pool::getRequired: handle points to a dead/stale slot");
            return m_slots[handle.index()].value;
        }

        bool remove(OptionalHandle<T> handle) noexcept
        {
            if (!isValidSlot(handle)) return false;

            Slot &slot = m_slots[handle.index()];
            slot.alive = false;
            slot.generation++;          
            slot.value = T{};          
            m_freeList.push_back(handle.index());
            return true;
        }

        bool contains(OptionalHandle<T> handle) const noexcept
        {
            return isValidSlot(handle);
        }

        size_t size() const noexcept { return m_slots.size() - m_freeList.size(); }
        size_t capacity() const noexcept { return m_slots.size(); }

        template<typename Fn>
        void forEach(Fn &&fn)
        {
            for (uint32_t i = 0; i < m_slots.size(); ++i) {
                if (m_slots[i].alive) {
                    fn(Handle<T>(i, m_slots[i].generation), m_slots[i].value);
                }
            }
        }

    private:
        struct Slot {
            T value{};
            uint32_t generation = 0;
            bool alive = false;
        };

        bool isValidSlot(const Handle<T> &handle) const noexcept
        {
            if (!handle.isValid()) return false;
            if (handle.index() >= m_slots.size()) return false;
            const Slot &slot = m_slots[handle.index()];
            return slot.alive && slot.generation == handle.generation();
        }

        std::vector<Slot> m_slots;
        std::vector<uint32_t> m_freeList;
    };

} // namespace SM

#endif // SM_CORE_POOL_H_
