/** @file stack_allocator.hpp */

#pragma once

// module includes
#include "allocator_interface.hpp"
#include "block.hpp"
#include "util/util.hpp"
#include "utils.hpp"


namespace memory
{
    template <usize capacity, usize align = NO_ALIGN>
    class StackAllocator : public AllocatorInterface
    {
    private:
        u8 m_buf[capacity] = { 0 };

        u8 *m_ptr = { align_front<align>(m_buf) };

    public:
        auto alloc(usize size) -> Block override;
        auto owns(Block &block) const noexcept -> bool override;
        auto free(Block &block) -> void override;
    };
}  // namespace memory
