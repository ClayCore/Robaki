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
        auto static constexpr const node_size = sizeof(T::Node);

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

            auto inline operator==(iterator const &other) const noexcept -> bool;
            auto inline operator!=(iterator const &other) const noexcept -> bool;

            auto inline operator++() noexcept -> iterator &;
            auto inline operator++(i32) const noexcept -> iterator;
            auto inline operator*() const noexcept -> value_type;
        };

        explicit FreeListHelper(T &instance);

        auto &get_allocator() noexcept;

        [[nodiscard]] auto begin() -> iterator;
        [[nodiscard]] auto end() -> iterator;
    };
}  // namespace memory::detail

namespace memory
{
    template <class Allocator, usize BS, usize Min, usize Max, usize Cap>
    class FreeList: public AllocatorInterface
    {
    private:
        using self = FreeList<Allocator, BS, Min, Max, Cap>;
        friend detail::FreeListHelper<self>;

        struct Node
        {
            Block block;
            Node *next;
        };

        Allocator m_allocator {};
        Node *m_head { nullptr };
        usize m_allocs { 0U };

    public:
        using allocator_type = Allocator;

        usize static constexpr const batch_size { BS };
        usize static constexpr const min { Min };
        usize static constexpr const max { Max };
        usize static constexpr const capacity { Cap };

        auto alloc(usize size) -> Block override;
        auto owns(Block &block) const noexcept -> bool override;
        auto free(Block &block) -> void override;
    };
}  // namespace memory
