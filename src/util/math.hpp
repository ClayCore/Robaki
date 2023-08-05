/** @file math.hpp
 * Various math utilities
 */

#pragma once

// module includes
#include "traits.hpp"
#include "types.hpp"

// c++ includes
#include <algorithm>
#include <numeric>

namespace util::math
{
    template <typename T, usize Count>
    requires std::is_arithmetic_v<T>
    struct Vec
    {
    private:
        std::array<T, Count> m_data;

        // ========================================================================================== //
        // Arithmetic methods ======================================================================= //
        // ========================================================================================== //

        auto add(T const &value) -> void
        requires Addable<T>
        {
            // clang-format off
            std::for_each(m_data.begin(), m_data.end(), [&value](T &elem) {
                elem += value;
            });
            // clang-format on
        }

        auto add(Vec<T, Count> const &vec) -> void
        requires Addable<T>
        {
            usize counter = { 0U };

            // clang-format off
            std::for_each(m_data.begin(), m_data.end(), [&counter, &vec](T &elem) {
                elem += vec.m_data[counter++];
            });
            // clang-format on
        }

        auto sub(T const &value) -> void
        requires Subtractable<T>
        {
            // clang-format off
            std::for_each(m_data.begin(), m_data.end(), [&value](T &elem) {
                elem -= value;
            });
            // clang-format on
        }

        auto sub(Vec<T, Count> const &vec) -> void
        requires Subtractable<T>
        {
            usize counter = { 0U };

            // clang-format off
            std::for_each(m_data.begin(), m_data.end(), [&counter, &vec](T &elem) {
                elem -= vec.m_data[counter++];
            });
            // clang-format on
        }

        auto mul(T const &value) -> void
        requires Multiplicable<T>
        {
            // clang-format off
            std::for_each(m_data.begin(), m_data.end(), [&value](T &elem) {
                elem *= value;
            });
            // clang-format on
        }

        auto mul(Vec<T, Count> const &vec) -> void
        requires Multiplicable<T>
        {
            usize counter = { 0U };

            // clang-format off
            std::for_each(m_data.begin(), m_data.end(), [&counter, &vec](T &elem) {
                elem *= vec.m_data[counter++];
            });
            // clang-format on
        }

        auto div(T const &value) -> void
        requires Divisible<T>
        {
            // clang-format off
            std::for_each(m_data.begin(), m_data.end(), [&value](T &elem) {
                elem /= value;
            });
            // clang-format on
        }

        auto div(Vec<T, Count> const &vec) -> void
        requires Divisible<T>
        {
            usize counter = { 0U };

            // clang-format off
            std::for_each(m_data.begin(), m_data.end(), [&counter, &vec](T &elem) {
                elem /= vec.m_data[counter++];
            });
            // clang-format on
        }

    public:
        // ========================================================================================== //
        // Type aliases ============================================================================= //
        // ========================================================================================== //

        // ========================================================================================== //
        // Constructors ============================================================================= //
        // ========================================================================================== //

        Vec() = default;

        template <typename... Args>
        explicit Vec(Args... args) : m_data{ { args... } }
        {
            constexpr usize dims = sizeof...(args);
            static_assert(dims == Count);
        }

        // ========================================================================================== //
        // Mutators and transformer methods ========================================================= //
        // ========================================================================================== //

        auto pop() -> Vec<T, Count - 1>
        {
            Vec<T, Count - 1> result = {};

            std::copy_n(m_data.begin(), Count - 1, result.m_data.begin());

            return (result);
        }

        auto push(T const &value) -> Vec<T, Count + 1>
        {
            Vec<T, Count + 1> result = {};

            std::copy_n(m_data.begin(), Count + 1, result.m_data.begin());
            result.m_data[Count] = value;

            return (result);
        }

        template <usize Index>
        constexpr auto get_value() const -> T &
        {
            return (m_data[Index]);
        }

        constexpr auto get_value(usize index) const -> T &
        {
            return (this->get_value<index>());
        }

        template <typename... Args>
        auto set(Args... args) -> void
        {
            constexpr usize dims = sizeof...(args);
            static_assert(dims == Count);

            m_data = { { args... } };
        }

        auto length() -> void
        requires Addable<T> && Multiplicable<T>
        {
            auto square = m_data;

            // clang-format off
            std::for_each(square.begin(), square.end(), [](T &elem) {
                elem = (elem * elem);
            });
            // clang-format on

            auto sum = std::accumulate(square.begin(), square.end(), static_cast<T>(0U));

            if constexpr (std::is_floating_point_v<T>) {
                return (std::sqrtf(sum));
            } else {
                return (std::sqrt(sum));
            }
        }

        auto norm() -> void
        requires Addable<T> && Multiplicable<T>
        {
            auto len = 1.0 / this->length();

            // clang-format off
            std::for_each(m_data.begin(), m_data.end(), [&len](T &elem) { elem *= len; });
            // clang-format on
        }

        // ========================================================================================== //
        // Element aliases ========================================================================== //
        // ========================================================================================== //

        constexpr auto x() -> T &
        {
            static_assert(1 <= Count, "Invalid number of arguments for vector type");
            return (this->get_value<0>());
        }

        constexpr auto y() -> T &
        {
            static_assert(2 <= Count, "Invalid number of arguments for vector type");
            return (this->get_value<1>());
        }

        constexpr auto z() -> T &
        {
            static_assert(3 <= Count, "Invalid number of arguments for vector type");
            return (this->get_value<2>());
        }

        constexpr auto w() -> T &
        {
            static_assert(4 <= Count, "Invalid number of arguments for vector type");
            return (this->get_value<3>());
        }


        // ========================================================================================== //
        // Operator overloads ======================================================================= //
        // ========================================================================================== //

        auto operator+=(Vec<T, Count> const &other) -> Vec<T, Count> &
        requires Multiplicable<T>
        {
            this->add(other);

            return (*this);
        }

        friend inline auto operator+(Vec<T, Count> lhs, Vec<T, Count> const &rhs) -> Vec<T, Count>
        requires Addable<T>
        {
            lhs += rhs;

            return (lhs);
        }

        auto operator-=(Vec<T, Count> const &other) -> Vec<T, Count> &
        requires Subtractable<T>
        {
            this->sub(other);

            return (*this);
        }

        friend inline auto operator-(Vec<T, Count> lhs, Vec<T, Count> const &rhs) -> Vec<T, Count>
        requires Subtractable<T>
        {
            lhs -= rhs;

            return (lhs);
        }

        auto operator*=(Vec<T, Count> const &other) -> Vec<T, Count> &
        requires Multiplicable<T>
        {
            this->mul(other);

            return (*this);
        }

        friend inline auto operator*(Vec<T, Count> lhs, Vec<T, Count> const &rhs) -> Vec<T, Count>
        requires Multiplicable<T>
        {
            lhs *= rhs;

            return (lhs);
        }

        auto operator/=(Vec<T, Count> const &other) -> Vec<T, Count> &
        requires Divisible<T>
        {
            this->div(other);

            return (*this);
        }

        friend inline auto operator/(Vec<T, Count> lhs, Vec<T, Count> const &rhs) -> Vec<T, Count>
        requires Divisible<T>
        {
            lhs /= rhs;

            return (lhs);
        }
    };

    template <typename T>
    requires std::is_arithmetic_v<T>
    using Vec2 = Vec<T, 2>;

    template <typename T>
    requires std::is_arithmetic_v<T>
    using Vec3 = Vec<T, 3>;

    template <typename T>
    requires std::is_arithmetic_v<T>
    using Vec4 = Vec<T, 4>;
}  // namespace util::math

using namespace util::math;
