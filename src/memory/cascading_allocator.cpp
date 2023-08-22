/** @file cascading_allocator.cpp */

// module includes
#include "cascading_allocator.hpp"


namespace memory
{
    template <IsAllocator... Allocators>
    auto CascadingAllocator<Allocators...>::alloc(usize size) -> Block
    {
        if (m_list.empty()) {
            return (null_block);
        }

        Block result {};
        for (auto &node : m_list) {
            result = std::visit([size](auto &&arg) { return (arg.alloc(size)); }, node.first);

            if (result == null_block) {
                break;
            }
            if (!node.second) {
                break;
            }
        }

        return (result);
    }

    template <IsAllocator... Allocators>
    auto CascadingAllocator<Allocators...>::owns(Block &block) const noexcept -> bool
    {
        if (m_list.empty()) {
            return (block == null_block);
        }

        for (auto &node : m_list) {
            auto owned = std::visit([&block](auto &&arg) -> bool { return (arg.owns(block)); }, node);
            if (!node.second) {
                return (false);
            }

            return (owned);
        }

        return (false);
    }

    template <IsAllocator... Allocators>
    auto CascadingAllocator<Allocators...>::free(Block &block) -> void
    {
        if (m_list.empty() && (block != null_block)) {
            throw std::runtime_error("Cannot free block not owned by any known allocators");
        }

        for (auto &node : m_list) {
            auto owned = std::visit([&block](auto &&arg) { return (arg.owns(block)); }, node.first);

            if (owned) {
                std::visit([&block](auto &&arg) { arg.free(block); }, node.first);
            }

            if (!node.second) {
                break;
            }
        }
    }
}  // namespace memory
