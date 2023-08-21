/** @file affix_allocator.cpp */

// module includes
#include "affix_allocator.hpp"

namespace memory::detail
{
    // ============================================================================================== //
    // Utility functions ============================================================================ //
    // ============================================================================================== //

    template <class T>
    auto constexpr size_of() -> usize
    {
        return (sizeof(T));
    }

    template <>
    auto constexpr size_of<void>() -> usize
    {
        return (0U);
    }

    template <class T>
    auto inline construct_at(Block &block, usize offset) -> void
    {
        (new (block + offset) T {});
    }

    template <>
    auto inline construct_at<void>(Block &block, usize offset) -> void
    {
    }

    template <class T>
    auto inline destruct_at(Block &block, usize offset) -> void
    {
        reinterpret_cast<T *>(block + offset)->~T();
    }

    template <>
    auto inline destruct_at<void>(Block &block, usize offset) -> void
    {
    }

    // ============================================================================================== //
    // Helper implementation ======================================================================== //
    // ============================================================================================== //

    template <class T>
    inline AffixAllocatorHelper<T>::AffixAllocatorHelper(T &instance): m_instance { instance }
    {
    }

    template <class T>
    auto inline &AffixAllocatorHelper<T>::get_allocator()
    {
        return (m_instance.m_allocator);
    }
}  // namespace memory::detail

namespace memory
{
    // ============================================================================================== //
    // Static method implementation ================================================================= //
    // ============================================================================================== //

    template <class Allocator, class P, class S, bool V>
    auto inline AffixAllocator<Allocator, P, S, V>::size_of_alloc(usize size) noexcept -> usize
    {
        using namespace detail;

        return (size + size_of<P>() + size_of<S>());
    }

    template <class Allocator, class P, class S, bool V>
    auto inline AffixAllocator<Allocator, P, S, V>::offset_block(Block &block, usize size) noexcept -> Block
    {
        using namespace detail;

        Block offset = { block + size_of<P>(), size };
        return (offset);
    }

    template <class Allocator, class P, class S, bool V>
    auto inline AffixAllocator<Allocator, P, S, V>::original_block(Block &block) noexcept -> Block
    {
        using namespace detail;

        Block original = { block - size_of<P>(), block.size + size_of<P>() + size_of<S>() };
        return (original);
    }

    template <class Allocator, class P, class S, bool V>
    auto inline AffixAllocator<Allocator, P, S, V>::construct_prefix(Block &block) -> void
    {
        detail::construct_at<P>(block, 0U);
    }

    template <class Allocator, class P, class S, bool V>
    auto inline AffixAllocator<Allocator, P, S, V>::construct_suffix(Block &block) -> void
    {
        detail::construct_at<S>(block, block.size - detail::size_of<S>());
    }

    template <class Allocator, class P, class S, bool V>
    auto inline AffixAllocator<Allocator, P, S, V>::verify_prefix(Block &block) -> void
    {
        detail::destruct_at<P>(block, 0U);
    }

    template <class Allocator, class P, class S, bool V>
    auto inline AffixAllocator<Allocator, P, S, V>::verify_suffix(Block &block) -> void
    {
        detail::destruct_at<S>(block, block.size - detail::size_of<S>());
    }

    // ============================================================================================== //
    // AffixAllocator implementation ================================================================ //
    // ============================================================================================== //

    template <class Allocator, class P, class s, bool V>
    auto AffixAllocator<Allocator, P, s, V>::alloc(usize size) -> Block
    {
        auto block = m_allocator.alloc(self::size_of_alloc(size));
        if (block == null_block) {
            return (block);
        }

        self::construct_prefix(block);
        self::construct_suffix(block);

        return (self::offset_block(block, size));
    }

    template <class Allocator, class P, class s, bool V>
    auto AffixAllocator<Allocator, P, s, V>::owns(Block &block) const noexcept -> bool
    {
        auto original = self::original_block(block);

        return (m_allocator.owns(original));
    }

    template <class Allocator, class P, class s, bool V>
    auto AffixAllocator<Allocator, P, s, V>::free(Block &block) -> void
    {
        auto original = self::original_block(block);

        if (m_allocator.owns(original)) {
            if (V) {
                self::verify_prefix(original);
                self::verify_prefix(original);
            }

            m_allocator.free(original);

            block = original;
        }
    }
}  // namespace memory
