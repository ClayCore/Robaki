/** @file null_allocator.cpp */

// module includes
#include "null_allocator.hpp"

namespace memory
{
    auto NullAllocator::alloc(usize /*size*/) -> Block
    {
        return (null_block);
    }

    auto NullAllocator::owns(Block &block) const noexcept -> bool
    {
        return (block.addr == nullptr);
    }

    auto NullAllocator::free(Block &block) -> void
    {
        block.addr = nullptr;
        block.size = 0U;
    }
}  // namespace memory
