/** @file null_allocator.hpp */

#pragma once

// module includes
#include "allocator_interface.hpp"
#include "block.hpp"
#include "util/util.hpp"
#include "utils.hpp"


namespace memory
{
    class NullAllocator : public AllocatorInterface
    {
    public:
        auto alloc(usize size = 0) -> Block override;
        auto owns(Block &block) const noexcept -> bool override;
        auto free(Block &block) -> void override;
    };
}  // namespace memory
