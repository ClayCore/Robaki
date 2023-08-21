/** @file traits.hpp
 * Utility traits
 */

#pragma once

// module includes
#include "types.hpp"

// c++ includes
#include <chrono>
#include <concepts>
#include <iostream>
#include <tuple>
#include <type_traits>
#include <vector>


namespace util::traits
{
    // ================================================================================================== //
    // Forward declarations in use for traits and concepts ============================================== //
    // ================================================================================================== //
    struct Block;

    // ================================================================================================== //
    // Type related traits ============================================================================== //
    // ================================================================================================== //

    /*******************************************************************************************************
     * Validate if a given type is `std::vector`
     *
     * @tparam T type to validate
     ******************************************************************************************************/
    template <class T>
    struct is_vector: public std::false_type
    {
    };

    template <class T>
    struct is_vector<std::vector<T>>: public std::true_type
    {
    };

    /*******************************************************************************************************
     * Determine if type `T` is `std::chrono::duration`
     *
     * @tparam T type fo validate
     ******************************************************************************************************/
    template <class T>
    struct is_duration: public std::false_type
    {
    };

    template <class R, class P>
    struct is_duration<std::chrono::duration<R, P>>: public std::true_type
    {
    };

    /*******************************************************************************************************
     * Constexpr alias for whether `T` is `std::chrono::duration`
     *
     * @tparam T type to validate
     ******************************************************************************************************/
    template <class T>
    bool constexpr const is_duration_v = is_duration<T>::value;

    /*******************************************************************************************************
     * Constexpr alias for whether a type's an `std::vector`
     *
     * @tparam T type to validate
     ******************************************************************************************************/
    template <class T>
    bool constexpr const is_vector_v = is_vector<T>::value;

    /*******************************************************************************************************
     * Alias for `is_duration_v` as a concept
     *
     * @tparam T type to validate
     ******************************************************************************************************/
    template <class T>
    concept IsDuration = is_duration_v<T>;

    /*******************************************************************************************************
     * Constraints type `T` to be a vector
     *
     * @tparam T type to constrain
     ******************************************************************************************************/
    template <class T>
    concept IsVector = is_vector_v<T>;

    /*******************************************************************************************************
     * Alias for `std::is_arithmetic_v` as a concept
     *
     * @tparam T type to constrain
     ******************************************************************************************************/
    template <typename T>
    concept Arithmetic = std::is_arithmetic_v<T>;

    /*******************************************************************************************************
     * Constrain `T` to be printable
     *
     * @tparam T type to constrain
     ******************************************************************************************************/
    template <class T>
    concept IsPrintable = requires(T type) {
        {
            std::cout << type
        };
    };

    /*******************************************************************************************************
     * Constrain `T` to an allocator
     *
     * @tparam T type to constrain
     ******************************************************************************************************/
    template <class T>
    concept IsAllocator = requires(T type) {
        // clang-format off
        { type.alloc(0) };
        { type.owns(std::declval<Block>) };
        { type.free(std::declval<Block>) };
        // clang-format on
    };

    /*******************************************************************************************************
     * Constrain `T` to an STL container
     *
     * @tparam T type to constrain
     ******************************************************************************************************/
    template <class T>
    concept IsContainer = requires(T type) {
        // clang-format off
        { std::begin(type)                   };
        { std::begin(type)++                 };
        { std::end(type)                     };
        { *std::begin(type)                  };
        { *std::begin(type)                  } -> std::same_as<void>;
        { std::begin(type) != std::end(type) } -> std::same_as<bool>;
        std::destructible<decltype(std::begin(type))> &&
        std::destructible<decltype(std::end(type))> &&
        std::copy_constructible<decltype(std::begin(type))> &&
        std::copy_constructible<decltype(std::end(type))>;
        // clang-format on
    };

    // ================================================================================================== //
    // Reversing order of arguments in templates ======================================================== //
    // ================================================================================================== //

    // 1. Reversing a type sequence ===================================================================== //
    // ================================================================================================== //

    template <typename, typename>
    struct append_to_type_seq
    {
    };

    template <typename T, typename... Args>
    struct append_to_type_seq<T, std::tuple<Args...>>
    {
        using type = std::tuple<Args..., T>;
    };

    template <typename... Args>
    struct reverse_type_seq
    {
        using type = std::tuple<>;
    };

    template <typename T, typename... Args>
    struct reverse_type_seq<T, Args...>
    {
        using type = typename append_to_type_seq<T, typename reverse_type_seq<Args...>::type>::type;
    };

    // 2. Reversing a tuple ============================================================================= //
    // ================================================================================================== //

    template <usize... Indices>
    struct index_list
    {
    };

    namespace detail
    {
        template <usize Min, usize Count, usize... Indices>
        struct range_builder;

        template <usize Min, usize... Indices>
        struct range_builder<Min, Min, Indices...>
        {
            using type = index_list<Indices...>;
        };

        template <usize Min, usize Count, usize... Indices>
        struct range_builder: public range_builder<Min, Count - 1, Count - 1, Indices...>
        {
        };
    };  // namespace detail

    template <usize Min, usize Max>
    using index_range_t = typename detail::range_builder<Min, Max>::type;

    template <typename... Args, usize... Indices>
    auto constexpr reverse_tuple(std::tuple<Args...> tuple, index_list<Indices...> /* unused */)
        -> reverse_type_seq<Args...>::type
    {
        using reversed_tuple = typename reverse_type_seq<Args...>::type;

        auto reverse_tuple = std::forward_as_tuple(
            std::forward<
                std::conditional_t<std::is_lvalue_reference_v<std::tuple_element_t<Indices, reversed_tuple>>,
                                   std::tuple_element_t<Indices, reversed_tuple>,
                                   std::remove_reference_t<std::tuple_element_t<Indices, reversed_tuple>>>>(
                std::get<sizeof...(Args) - Indices - 1>(tuple))...);

        return (reverse_tuple);
    }

    template <typename... Args>
    auto reverse_tuple(std::tuple<Args...> tuple) -> reverse_type_seq<Args...>::type
    {
        return (reverse_tuple(tuple, index_range_t<0, sizeof...(Args)>()));
    }

    // 3. Reversing function arguments ================================================================== //
    // ================================================================================================== //

    template <typename... Args>
    auto make_revert(Args &&...args) -> reverse_type_seq<Args...>::type
    {
        auto tuple = std::forward_as_tuple(std::forward<Args>(args)...);

        return (reverse_tuple(tuple));
    }

    // ================================================================================================== //
    // Event related traits ============================================================================= //
    // ================================================================================================== //

    // Necessary for the constraints below
    class Event;

    /*******************************************************************************************************
     * Constrain `Derived` so that it *must* implement `this->emit(event)`
     * Used by `Provider` and `Emitter` module
     *
     * @tparam Derived constrainted type
     ******************************************************************************************************/
    template <typename Derived>
    concept Emittable = requires(Derived &derived, Event const &event) { derived.emit(event); };

    /*******************************************************************************************************
     * Constrain `Derived` so that it *must* implement `this->dispatch(event)`
     * Used by `Provider` and `Dispatcher` module
     *
     * @tparam Derived constrainted type
     ******************************************************************************************************/
    template <typename Derived>
    concept Dispatchable = requires(Derived &derived, Event const &event) { derived.dispatch(event); };

    /*******************************************************************************************************
     * Constrain `Derived` so that it *must* implement `this->listen(event)`
     * Used by `Provider` and `Listener` module
     *
     * @tparam Derived constrainted type
     ******************************************************************************************************/
    template <typename Derived>
    concept Listenable = requires(Derived &derived, Event const &event) { derived.listen(event); };

    // ================================================================================================== //
    // Arithmetic traits ================================================================================ //
    // ================================================================================================== //

    /*******************************************************************************************************
     * Constrain a given type to support arithmetic addition
     * Used by `util::math` module
     *
     * @tparam T constrained type
     ******************************************************************************************************/
    template <typename T>
    concept AddableUnary = requires(T value) {
        // clang-format off
        { (value + value)   };
        { (value += value)  };
        { (value++)         };
        { (++value)         };
        // clang-format on
    };

    /*******************************************************************************************************
     * Constrain 2 types to support arithmetic addition
     * Used by `util::math` module
     *
     * @tparam T constrained type
     * @tparam U constrained type
     ******************************************************************************************************/
    template <typename T, typename U>
    concept AddableBinary = requires(T lhs, U rhs) {
        // clang-format off
        { (lhs + rhs)       };
        { (lhs += rhs)      };
        { (rhs += lhs)      };
        // clang-format on
    };

    /*******************************************************************************************************
     * Constrain a given type to support arithmetic subtraction
     * Used by `util::math` module
     *
     * @tparam T constrained type
     ******************************************************************************************************/
    template <typename T>
    concept SubtractableUnary = requires(T value) {
        // clang-format off
        { (value - value)   };
        { (value -= value)  };
        { (value--)         };
        { (--value)         };
        // clang-format on
    };

    /*******************************************************************************************************
     * Constrain 2 types to support arithmetic subtraction
     * Used by `util::math` module
     *
     * @tparam T constrained type
     * @tparam U constrained type
     ******************************************************************************************************/
    template <typename T, typename U>
    concept SubtractableBinary = requires(T lhs, U rhs) {
        // clang-format off
        { (lhs - rhs)       };
        { (lhs -= rhs)      };
        { (rhs -= lhs)      };
        // clang-format on
    };

    /*******************************************************************************************************
     * Constrain a given type to support arithmetic multiplication
     * Used by `util::math` module
     *
     * @tparam T constrained type
     ******************************************************************************************************/
    template <typename T>
    concept MultiplicableUnary = requires(T value) {
        // clang-format off
        { (value * value)   };
        { (value *= value)  };
        // clang-format on
    };

    /*******************************************************************************************************
     * Constrain 2 types to support arithmetic multiplication
     * Used by `util::math` module
     *
     * @tparam T constrained type
     * @tparam U constrained type
     ******************************************************************************************************/
    template <typename T, typename U>
    concept MultiplicableBinary = requires(T lhs, U rhs) {
        // clang-format off
        { (lhs * rhs)       };
        { (lhs *= rhs)      };
        // clang-format on
    };

    /*******************************************************************************************************
     * Constrain a given type to support arithmetic division
     * Used by `util::math` module
     *
     * @tparam T constrained type
     ******************************************************************************************************/
    template <typename T>
    concept DivisibleUnary = requires(T value) {
        // clang-format off
        { (value / value)   };
        { (value /= value)  };
        // clang-format on
    };

    /*******************************************************************************************************
     * Constrain 2 types to support arithmetic division
     * Used by `util::math` module
     *
     * @tparam T constrained type
     * @tparam U constrained type
     ******************************************************************************************************/
    template <typename T, typename U>
    concept DivisibleBinary = requires(T lhs, U rhs) {
        // clang-format off
        { (lhs / rhs)       };
        { (lhs /= rhs)      };
        // clang-format on
    };
}  // namespace util::traits

using namespace util::traits;
