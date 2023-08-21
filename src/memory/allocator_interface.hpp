/** @file allocator_interface.hpp */

#pragma once

// module includes
#include "util/util.hpp"
#include "utils.hpp"

namespace memory
{
    // forward reference
    class Block;

    /** Describes an abstract allocator interface */
    class AllocatorInterface: private NonCopyable
    {
    public:
        virtual ~AllocatorInterface() = default;

        /** Allocates a block with a given size */
        auto virtual alloc(usize size) -> Block = 0;

        /** Determine if a block belongs to this allocator */
        auto virtual owns(Block &block) const noexcept -> bool = 0;

        /** Deallocate the selected block */
        auto virtual free(Block &block) -> void = 0;
    };
}  // namespace memory
