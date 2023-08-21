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

        auto virtual alloc(usize size) -> Block = 0;

        auto virtual owns(Block &block) const noexcept -> bool = 0;

        auto virtual free(Block &block) -> void = 0;
    };
}  // namespace memory
