/** @file allocator_interface.hpp */

#pragma once

// module includes
#include "block.hpp"
#include "util/util.hpp"
#include "utils.hpp"


namespace memory
{
    class AllocatorInterface: private NonCopyable
    {
    public:
        virtual ~AllocatorInterface() = default;

        virtual auto alloc(usize size) -> Block = 0;

        virtual auto owns(Block &block) const noexcept -> bool = 0;

        virtual auto free(Block &block) -> void = 0;
    };
}  // namespace memory
