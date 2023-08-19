/** @file block.hpp */

#pragma once

// module includes
#include "util/util.hpp"
#include "utils.hpp"

// c++ includes
namespace memory
{
    struct Block
    {
        void *addr;
        usize size;

        explicit inline operator u8 *() const
        {
            return (reinterpret_cast<u8 *>(addr));
        }

        inline auto operator==(Block const &other) const -> bool
        {
            return (addr == other.addr) && (size == other.size);
        }
    };

    static const Block null_block = { nullptr, 0U };
}  // namespace memory
