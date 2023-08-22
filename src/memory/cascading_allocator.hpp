/** @file cascading_allocator.hpp */

#pragma once

// module includes
#include "allocator_interface.hpp"
#include "block.hpp"
#include "util/util.hpp"
#include "utils.hpp"

// c++ includes
#include <list>
#include <utility>
#include <variant>


namespace memory
{
    template <IsAllocator... Allocators>
    class CascadingAllocator: public AllocatorInterface
    {
    private:
        using Node = std::pair<std::variant<Allocators...>, bool>;

        std::list<Node> m_list;

    public:
        auto alloc(usize size) -> Block override;
        auto owns(Block &block) const noexcept -> bool override;
        auto free(Block &block) -> void override;
    };
}  // namespace memory
