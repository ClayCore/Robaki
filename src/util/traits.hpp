/** @file traits.hpp
 * Utility traits
 */

#pragma once

// c++ includes
#include <concepts>
#include <iostream>

// module includes
#include "types.hpp"

namespace util::traits
{
    template <typename T>
    struct is_printable
    {
        template <typename U, typename = decltype(std::cout << std::declval<U>())>
        static constexpr auto validate(void *) -> std::true_type;

        template <typename U>
        static constexpr auto validate(...) -> std::false_type;

        static constexpr bool value = std::is_same_v<decltype(validate<T>(nullptr)), std::true_type>;
    };

    template <typename T>
    constexpr inline bool is_printable_v = is_printable<T>::value;
}  // namespace util::traits
