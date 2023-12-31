/** @file block.hpp */

#pragma once

// module includes
#include "util/util.hpp"
#include "utils.hpp"

// c++ includes
namespace memory
{
    /**
     * Describes an allocated memory block
     */
    struct Block
    {
        /** Block address */
        void *addr;
        /** Block size */
        usize size;

        /** Converts the block into the underlying address */
        inline operator u8 *() const
        {
            return (reinterpret_cast<u8 *>(addr));
        }

        /** Checks if two blocks are identical */
        auto inline operator==(Block const &other) const -> bool
        {
            return (addr == other.addr) && (size == other.size);
        }
    };

    /** Invalid memory block */
    Block static const null_block = { nullptr, 0U };
}  // namespace memory
