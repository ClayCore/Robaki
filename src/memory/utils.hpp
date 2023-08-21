/** @file utils.hpp */

#pragma once

// module includes
#include "util/util.hpp"

namespace memory
{
    usize static constexpr const NO_ALIGN = { 1U };

    // clang-format off

    #if defined(__amd64__) || defined(__aarch64__)
        static constexpr const usize WORD_ALIGN = {8U};
        static constexpr const usize HALF_WORD_ALIGN = (WORD_ALIGN / 2U);
    #elif defined(__i686) || defined(__arm__)
        static constexpr const usize WORD_ALIGN = {4U};
        static constexpr const usize HALF_WORD_ALIGN = (WORD_ALIGN / 2U);
    #endif

    // clang-format on

    auto inline constexpr is_power_of_two(usize num) noexcept -> bool
    {
        while (((num & 1U) == 0U) && (num > 1U)) {
            num >>= 1U;
        }

        return (num == 1U);
    }

    template <usize align>
    auto inline constexpr align_front(u8 *addr) noexcept -> u8 *
    {
        static_assert(is_power_of_two(align), "alignment must be power of two");

        auto addr_ptr = reinterpret_cast<uintptr>(addr);
        auto mod      = (addr_ptr % align);

        if ((addr_ptr % align) == 0U) {
            return (addr);
        } else {
            return (reinterpret_cast<u8 *>(addr_ptr + (align - mod)));
        }
    }

    template <>
    auto inline constexpr align_front<1U>(u8 *addr) noexcept -> u8 *
    {
        return (addr);
    }

    template <usize align>
    auto inline constexpr align_back(u8 *addr) noexcept -> u8 *
    {
        static_assert(is_power_of_two(align), "alignment must be power of two");

        auto addr_ptr = reinterpret_cast<uintptr>(addr);
        auto mod      = (addr_ptr % align);

        if ((addr_ptr % align) == 0U) {
            return (addr);
        } else {
            return (reinterpret_cast<u8 *>(addr_ptr - mod));
        }
    }

    template <>
    auto inline constexpr align_back<1U>(u8 *addr) noexcept -> u8 *
    {
        return (addr);
    }

    template <usize align>
    auto inline constexpr align_size(usize size) noexcept -> usize
    {
        static_assert(is_power_of_two(align), "alignment must be power of two");

        if constexpr ((size % align) == 0U) {
            return (size);
        } else {
            return (size + (align - (size % align)));
        }
    }

    template <>
    auto inline constexpr align_size<1U>(usize size) noexcept -> usize
    {
        return (size);
    }

}  // namespace memory
