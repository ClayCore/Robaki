/** @file traits.hpp
 * Utility traits
 */

#pragma once

// c++ includes
#include <concepts>
#include <iostream>
#include <type_traits>
#include <vector>


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

    template <typename T>
    struct is_vector
    {
        static constexpr const bool value = false;
    };

    template <typename T>
    struct is_vector<std::vector<T>>
    {
        static constexpr const bool value = true;
    };

    template <typename T>
    constexpr inline bool is_vector_v = is_vector<T>::value;

    // Necessary for the constraints below
    class Event;

    template <typename Derived>
    concept Emittable = requires(Derived &derived, Event const &event) { derived.emit(event); };

    template <typename Derived>
    concept Dispatchable = requires(Derived &derived, Event const &event) { derived.dispatch(event); };

    template <typename Derived>
    concept Listenable = requires(Derived &derived, Event const &event) { derived.listen(event); };

}  // namespace util::traits
