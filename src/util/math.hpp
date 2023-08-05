/** @file math.hpp
 * Various math utilities
 */

#pragma once

// module includes
#include "traits.hpp"
#include "types.hpp"
#include "util/traits.hpp"

// c++ includes
#include <algorithm>
#include <array>
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

        template <typename U>
        auto add(U const &value) -> void
        requires AddableBinary<T, U>
        {
            // clang-format off
            std::for_each(m_data.begin(), m_data.end(), [&value](T &elem) {
                elem += value;
            });
            // clang-format on
        }

        auto add(Vec<T, Count> const &vec) -> void
        requires AddableUnary<T>
        {
            usize counter = { 0U };

            // clang-format off
            std::for_each(m_data.begin(), m_data.end(), [&counter, &vec](T &elem) {
                elem += vec.m_data[counter++];
            });
            // clang-format on
        }

        template <typename U>
        auto sub(U const &value) -> void
        requires SubtractableBinary<T, U>
        {
            // clang-format off
            std::for_each(m_data.begin(), m_data.end(), [&value](T &elem) {
                elem -= value;
            });
            // clang-format on
        }

        auto sub(Vec<T, Count> const &vec) -> void
        requires SubtractableUnary<T>
        {
            usize counter = { 0U };

            // clang-format off
            std::for_each(m_data.begin(), m_data.end(), [&counter, &vec](T &elem) {
                elem -= vec.m_data[counter++];
            });
            // clang-format on
        }

        template <typename U>
        auto mul(U const &value) -> void
        requires MultiplicableBinary<T, U>
        {
            // clang-format off
            std::for_each(m_data.begin(), m_data.end(), [&value](T &elem) {
                elem *= value;
            });
            // clang-format on
        }

        auto mul(Vec<T, Count> const &vec) -> void
        requires MultiplicableUnary<T>
        {
            usize counter = { 0U };

            // clang-format off
            std::for_each(m_data.begin(), m_data.end(), [&counter, &vec](T &elem) {
                elem *= vec.m_data[counter++];
            });
            // clang-format on
        }

        template <typename U>
        auto div(U const &value) -> void
        requires DivisibleBinary<T, U>
        {
            // clang-format off
            std::for_each(m_data.begin(), m_data.end(), [&value](T &elem) {
                elem /= value;
            });
            // clang-format on
        }

        auto div(Vec<T, Count> const &vec) -> void
        requires DivisibleUnary<T>
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
        requires AddableUnary<T> && MultiplicableUnary<T>
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
        requires AddableUnary<T> && MultiplicableUnary<T>
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

        // 1. Addition ============================================================================== //
        // ========================================================================================== //

        /**
         * Adds the `other` vector in-place
         *
         * @param other vector to add
         * @return mutated instance
         */
        auto operator+=(Vec<T, Count> const &other) -> Vec<T, Count> &
        {
            this->add(other);

            return (*this);
        }

        /**
         * Adds `other` value in-place
         *
         * @tparam U value type
         * @param other value to add
         * @return mutated instance
         */
        template <typename U>
        auto operator+=(U const &other) -> Vec<T, Count> &
        {
            this->add(other);

            return (*this);
        }

        /**
         * Add `lhs` and `rhs` vectors and return the result
         *
         * @param lhs first operand
         * @param rhs second operand
         * @return result of addition
         */
        friend inline auto operator+(Vec<T, Count> lhs, Vec<T, Count> const &rhs) -> Vec<T, Count>
        {
            lhs += rhs;

            return (lhs);
        }

        /**
         * Add `lhs` vector and `rhs` value and return the result
         *
         * @tparam U value type
         * @param lhs first operand
         * @param rhs second operand
         * @return result of addition
         */
        template <typename U>
        friend inline auto operator+(Vec<T, Count> lhs, U const &rhs) -> Vec<T, Count>
        {
            lhs += rhs;

            return (lhs);
        }

        // 2. Subtraction =========================================================================== //
        // ========================================================================================== //

        /**
         * Subtracts the `other` vector in-place
         *
         * @param other vector to subtract
         * @return mutated instance
         */
        auto operator-=(Vec<T, Count> const &other) -> Vec<T, Count> &
        {
            this->sub(other);

            return (*this);
        }

        /**
         * Subtracts `other` value in-place
         *
         * @tparam U value type
         * @param other value to subtract
         * @return mutated instance
         */
        template <typename U>
        auto operator-=(U const &other) -> Vec<T, Count> &
        {
            this->sub(other);

            return (*this);
        }

        /**
         * Subtract `lhs` and `rhs` vectors and return the result
         *
         * @param lhs first operand
         * @param rhs second operand
         * @return result of subtraction
         */
        friend inline auto operator-(Vec<T, Count> lhs, Vec<T, Count> const &rhs) -> Vec<T, Count>
        {
            lhs -= rhs;

            return (lhs);
        }

        /**
         * Subtract `lhs` vector and `rhs` value and return the result
         *
         * @tparam U value type
         * @param lhs first operand
         * @param rhs second operand
         * @return result of subtraction
         */
        template <typename U>
        friend inline auto operator-(Vec<T, Count> lhs, U const &rhs) -> Vec<T, Count>
        {
            lhs -= rhs;

            return (lhs);
        }

        // 3. Multiplication ======================================================================== //
        // ========================================================================================== //

        /**
         * Multiplies with the `other` vector in-place
         *
         * @param other vector to multiply with
         * @return mutated instance
         */
        auto operator*=(Vec<T, Count> const &other) -> Vec<T, Count> &
        {
            this->mul(other);

            return (*this);
        }

        /**
         * Multiplies with the `other` value in-place
         *
         * @tparam U value type
         * @param other value to multiply with
         * @return mutated instance
         */
        template <typename U>
        auto operator*=(U const &other) -> Vec<T, Count> &
        {
            this->mul(other);

            return (*this);
        }

        /**
         * Mutiply `lhs` and `rhs` vectors and return the result
         *
         * @param lhs first operand
         * @param rhs second operand
         * @return result of multiplication
         */
        friend inline auto operator*(Vec<T, Count> lhs, Vec<T, Count> const &rhs) -> Vec<T, Count>
        {
            lhs *= rhs;

            return (lhs);
        }

        /**
         * Multiply `lhs` vector and `rhs` value and return the result
         *
         * @tparam U value type
         * @param lhs first operand
         * @param rhs second operand
         * @return result of multiplication
         */
        template <typename U>
        friend inline auto operator*(Vec<T, Count> lhs, U const &rhs) -> Vec<T, Count>
        {
            lhs *= rhs;

            return (lhs);
        }

        // 3. Division ============================================================================== //
        // ========================================================================================== //

        /**
         * Divides by the `other` vector in-place
         *
         * @param other vector to divide by
         * @return mutated instance
         */
        auto operator/=(Vec<T, Count> const &other) -> Vec<T, Count> &
        {
            this->div(other);

            return (*this);
        }

        /**
         * Divides by `other` value in-place
         *
         * @tparam U value type
         * @param other value to divide by
         * @return mutated instance
         */
        template <typename U>
        auto operator/=(U const &other) -> Vec<T, Count> &
        {
            this->div(other);

            return (*this);
        }

        /**
         * Divide `lhs` and `rhs` vectors and return the result
         *
         * @param lhs first operand
         * @param rhs second operand
         * @return result of division
         */
        friend inline auto operator/(Vec<T, Count> lhs, Vec<T, Count> const &rhs) -> Vec<T, Count>
        {
            lhs /= rhs;

            return (lhs);
        }

        /**
         * Divide `lhs` vector and `rhs` value and return the result
         *
         * @tparam U value type
         * @param lhs first operand
         * @param rhs second operand
         * @return result of division
         */
        template <typename U>
        friend inline auto operator/(Vec<T, Count> lhs, U const &rhs) -> Vec<T, Count>
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
