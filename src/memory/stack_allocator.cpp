/** @file stack_allocator.cpp */

// module includes
#include "stack_allocator.hpp"

namespace memory
{
    template <usize capacity, usize align>
    auto StackAllocator<capacity, align>::alloc(usize size) -> Block
    {
        auto end  = m_buf + capacity;
        auto next = align_front<align>(m_ptr + size);

        if (next <= end) {
            Block block { m_ptr, size };
            m_ptr = next;

            return (block);
        } else {
            return (null_block);
        }
    }

    template <usize capacity, usize align>
    auto StackAllocator<capacity, align>::owns(Block &block) const noexcept -> bool
    {
        constexpr auto from_front = (&m_buf <= block.addr);
        constexpr auto from_back  = (block.addr <= (&m_buf + capacity));

        return (from_front && from_back);
    }

    template <usize capacity, usize align>
    auto StackAllocator<capacity, align>::free(Block &block) -> void
    {
        auto prev = align_back<align>(m_ptr - block.size);

        if (prev == block.addr) {
            m_ptr = prev;
            block = { nullptr, 0U };
        }
    }
}  // namespace memory
