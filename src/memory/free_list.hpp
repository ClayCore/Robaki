/** @file free_list.hpp */

#pragma once

// module includes
#include "allocator_interface.hpp"
#include "block.hpp"
#include "util/util.hpp"
#include "utils.hpp"

// c++ includes
#include <iterator>


namespace memory::detail
{
    template <class T>
    class FreeListHelper final
    {
    private:
        T &m_instance;

    public:
        static constexpr auto node_size = sizeof(T::Node);

        class iterator
        {
        private:
            T::Node *m_current;

        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type        = Block;
            using difference_type   = usize;
            using pointer           = Block *;
            using reference         = Block &;

            explicit iterator(T::Node *current);

            inline auto operator==(iterator const &other) const noexcept -> bool;
            inline auto operator!=(iterator const &other) const noexcept -> bool;

            inline auto operator++() noexcept -> iterator &;
            inline auto operator++(i32) const noexcept -> iterator;
            inline auto operator*() const noexcept -> value_type;
        };

        explicit FreeListHelper(T &instance);

        auto &get_allocator() noexcept;

        auto begin() -> iterator;
        auto end() -> iterator;
    };
}  // namespace memory::detail

namespace memory
{
    template <class Allocator, usize batch_size, usize min, usize max, usize capacity>
    class FreeList: public AllocatorInterface
    {
    private:
        using self = FreeList<Allocator, batch_size, min, max, capacity>;
        friend detail::FreeListHelper<self>;

        struct Node
        {
            Block block;
            Node *next;
        };

        Allocator m_allocator = {};
        Node *m_head          = { nullptr };
        usize m_allocs        = { 0U };

    public:
        auto alloc(usize size) -> Block override;
        auto owns(Block &block) const noexcept -> bool override;
        auto free(Block &block) -> void override;
    };
}  // namespace memory
