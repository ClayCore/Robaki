/** @file free_list.cpp */

// module includes
#include "free_list.hpp"
#include "memory/block.hpp"
#include "memory/free_list.hpp"

namespace memory::detail
{
    template <class T>
    FreeListHelper<T>::iterator::iterator(T::Node *current): m_current(current)
    {
    }

    template <class T>
    inline auto FreeListHelper<T>::iterator::operator==(iterator const &other) const noexcept -> bool
    {
        return (m_current == other.m_current);
    }

    template <class T>
    inline auto FreeListHelper<T>::iterator::operator!=(iterator const &other) const noexcept -> bool
    {
        return !(*this == other);
    }

    template <class T>
    inline auto FreeListHelper<T>::iterator::operator++() noexcept -> iterator &
    {
        m_current = (m_current != nullptr) ? (m_current->next) : (nullptr);

        return (*this);
    }

    template <class T>
    inline auto FreeListHelper<T>::iterator::operator++(i32 step) const noexcept -> iterator
    {
        iterator advanced = { m_current };
        for (auto i = 0U; (i < step) && (advanced.m_current != nullptr); ++i) {
            advanced++;
        }

        return (advanced);
    }

    template <class T>
    inline auto FreeListHelper<T>::iterator::operator*() const noexcept -> value_type
    {
        return (m_current->block);
    }

    template <class T>
    FreeListHelper<T>::FreeListHelper(T &instance): m_instance(instance)
    {
    }

    template <class T>
    auto &FreeListHelper<T>::get_allocator() noexcept
    {
        return (m_instance.m_allocator);
    }

    template <class T>
    auto FreeListHelper<T>::begin() -> iterator
    {
        return (iterator { m_instance.m_head });
    }

    template <class T>
    auto FreeListHelper<T>::end() -> iterator
    {
        return (iterator { nullptr });
    }
}  // namespace memory::detail

namespace memory
{
#ifndef TEMPLATE_FREE_LIST
#define TEMPLATE_FREE_LIST template <class Allocator, usize batch_size, usize min, usize max, usize capacity>
#endif

    TEMPLATE_FREE_LIST
    auto FreeList<Allocator, batch_size, min, max, capacity>::alloc(usize size) -> Block
    {
        if ((size >= min) && (size <= max)) {
            if (m_head != nullptr) {
                auto block = m_head->block;
                m_head     = m_head->next;

                return (block);
            } else {
                Block first { nullptr, 0U };

                for (auto i = 0U; i < batch_size; ++i) {
                    if (m_allocs++ == capacity) {
                        break;
                    }

                    auto node_block = m_allocator.alloc(sizeof(Node) + max);
                    auto current    = reinterpret_cast<Node *>(node_block.addr);
                    current->block  = { node_block + sizeof(Node), max };

                    if (i == 0U) {
                        first = current->block;
                    } else {
                        current->next = m_head;
                        m_head        = current;
                    }
                }

                return (first);
            }
        }

        return (null_block);
    }

    TEMPLATE_FREE_LIST
    auto FreeList<Allocator, batch_size, min, max, capacity>::owns(Block &block) const noexcept -> bool
    {
        if ((block.size >= min) && (block.size <= max)) {
            return (m_allocator.owns(block));
        }

        return (false);
    }

    TEMPLATE_FREE_LIST
    auto FreeList<Allocator, batch_size, min, max, capacity>::free(Block &block) -> void
    {
        if (this->owns(block)) {
            // clang-format off
            auto current = reinterpret_cast<Node *>(
                reinterpret_cast<u8 *>(block.addr) - sizeof(Node)
            );
            // clang-format on

            current->next = m_head;
            m_head        = current;

            block = null_block;
        }
    }

#undef TEMPLATE_FREE_LIST
}  // namespace memory
