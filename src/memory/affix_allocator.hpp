/** @file affix_allocator.hpp */

#pragma once

// module includes
#include "allocator_interface.hpp"
#include "block.hpp"
#include "memory/free_list.hpp"
#include "util/util.hpp"
#include "utils.hpp"


namespace memory::detail
{
    template <class T>
    auto constexpr size_of() -> usize;
    template <>
    auto constexpr size_of<void>() -> usize;

    template <class T>
    auto inline construct_at(Block &block, usize offset) -> void;

    template <>
    auto inline construct_at<void>(Block &block, usize offset) -> void;

    template <class T>
    auto inline destruct_at(Block &block, usize offset) -> void;

    template <>
    auto inline destruct_at<void>(Block &block, usize offset) -> void;

    template <class T>
    class AffixAllocatorHelper final
    {
    private:
        T &m_instance;

    public:
        explicit inline AffixAllocatorHelper(T &instance);

        auto inline &get_allocator();
    };
}  // namespace memory::detail

namespace memory
{
    template <class Allocator, class P, class S = void, bool V = false>
    class AffixAllocator: public AllocatorInterface
    {
    private:
        using self = AffixAllocator<Allocator, P, S, V>;
        friend detail::FreeListHelper<self>;

        Allocator m_allocator;

        auto static inline size_of_alloc(usize size) noexcept -> usize;

        auto static inline offset_block(Block &block, usize size) noexcept -> Block;
        auto static inline original_block(Block &block) noexcept -> Block;

        auto static inline construct_prefix(Block &block) -> void;
        auto static inline construct_suffix(Block &block) -> void;

        auto static inline verify_prefix(Block &block) -> void;
        auto static inline verify_suffix(Block &block) -> void;

    public:
        using allocator_type = Allocator;
        using prefix_type    = P;
        using suffix_type    = S;

        bool static constexpr const verify { V };

        auto alloc(usize size) -> Block override;
        auto owns(Block &block) const noexcept -> bool override;
        auto free(Block &block) -> void override;
    };
}  // namespace memory
