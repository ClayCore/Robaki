/** @file fallback_allocator.cpp */

// module includes
#include "fallback_allocator.hpp"

namespace memory
{
    template <IsAllocator Primary, IsAllocator Secondary>
    auto FallbackAllocator<Primary, Secondary>::alloc(usize size) -> Block
    {
        auto block = m_primary.alloc(size);
        if (block.addr != nullptr) {
            return (block);
        } else {
            return (m_secondary.alloc(size));
        }
    }

    template <IsAllocator Primary, IsAllocator Secondary>
    auto FallbackAllocator<Primary, Secondary>::owns(Block &block) const noexcept -> bool
    {
        return (m_primary.owns(block) || m_secondary.owns(block));
    }

    template <IsAllocator Primary, IsAllocator Secondary>
    auto FallbackAllocator<Primary, Secondary>::free(Block &block) -> void
    {
        if (m_primary.owns(block)) {
            m_primary.free(block);
        } else {
            m_secondary.free(block);
        }
    }
    template <IsAllocator Primary, IsAllocator Secondary>
    auto inline FallbackAllocator<Primary, Secondary>::get_primary() noexcept -> Primary &
    {
        return (m_primary);
    }

    template <IsAllocator Primary, IsAllocator Secondary>
    auto inline FallbackAllocator<Primary, Secondary>::get_secondary() noexcept -> Secondary &
    {
        return (m_secondary);
    }

}  // namespace memory
