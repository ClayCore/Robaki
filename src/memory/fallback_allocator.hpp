/** @file fallback_allocator.hpp */

#pragma once

// module includes
#include "allocator_interface.hpp"
#include "block.hpp"
#include "util/util.hpp"
#include "utils.hpp"


namespace memory
{
    template <IsAllocator Primary, IsAllocator Secondary>
    class FallbackAllocator final: public AllocatorInterface
    {
    private:
        Primary m_primary;
        Secondary m_secondary;

    public:
        using primary_allocator_type   = Primary;
        using secondary_allocator_type = Secondary;

        auto alloc(usize size) -> Block override;
        auto owns(Block &block) const noexcept -> bool override;
        auto free(Block &block) -> void override;

        auto inline get_primary() noexcept -> Primary &;
        auto inline get_secondary() noexcept -> Secondary &;
    };
}  // namespace memory
