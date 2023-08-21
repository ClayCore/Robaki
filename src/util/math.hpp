/** @file math.hpp
 * Various math utilities
 */

#pragma once

// module includes
#include "demangle.hpp"
#include "traits.hpp"
#include "types.hpp"


// fmt includes
#ifndef FMT_HEADER_ONLY
#define FMT_HEADER_ONLY
#endif

#include "fmt/format.h"
#include "fmt/ranges.h"

// c++ includes
#include <algorithm>
#include <array>
#include <cmath>
#include <numeric>

namespace util::math
{
    // ============================================================================================== //
    // Math functions =============================================================================== //
    // ============================================================================================== //


    // ============================================================================================== //
    // Vector class ================================================================================= //
    // ============================================================================================== //


    /***************************************************************************************************
     * Generic N-Dim vector
     *
     * @tparam T value type (constrained to `Arithmetic`)
     * @tparam Count number of dimensions
     **************************************************************************************************/
    template <Arithmetic T, usize Count>
    struct Vec
    {
    private:
        // ========================================================================================== //
        // Arithmetic methods ======================================================================= //
        // ========================================================================================== //

        /***********************************************************************************************
         * Adds a value to this vector in-place
         *
         * Used by arithmetic operator overloads
         *
         * @tparam U value type
         * @param value value to add
         **********************************************************************************************/
        template <Arithmetic U>
        auto add(U const &value) -> void
        requires AddableBinary<T, U>
        {
            // clang-format off
            std::for_each(m_data.begin(), m_data.end(), [&value](T &elem) {
                elem += value;
            });
            // clang-format on
        }

        /***********************************************************************************************
         * Adds a vector to this vector in-place
         *
         * Used by arithmetic operator overloads
         *
         * @tparam U other vector value type
         * @param vec vector to add
         **********************************************************************************************/
        template <Arithmetic U>
        auto add(Vec<U, Count> const &vec) -> void
        requires AddableBinary<T, U>
        {
            usize counter = { 0U };

            // clang-format off
            std::for_each(m_data.begin(), m_data.end(), [&counter, &vec](T &elem) {
                elem += vec.m_data[counter++];
            });
            // clang-format on
        }

        /***********************************************************************************************
         * Subtracts a value from this vector in-place
         *
         * Used by arithmetic operator overloads
         *
         * @tparam U value type
         * @param value value to add
         **********************************************************************************************/
        template <Arithmetic U>
        auto sub(U const &value) -> void
        requires SubtractableBinary<T, U>
        {
            // clang-format off
            std::for_each(m_data.begin(), m_data.end(), [&value](T &elem) {
                elem -= value;
            });
            // clang-format on
        }

        /***********************************************************************************************
         * Subtracts a vector from this vector in-place
         *
         * Used by arithmetic operator overloads
         *
         * @tparam U other vector value type
         * @param vec vector to subtract out
         **********************************************************************************************/
        template <Arithmetic U>
        auto sub(Vec<U, Count> const &vec) -> void
        requires SubtractableBinary<T, U>
        {
            usize counter = { 0U };

            // clang-format off
            std::for_each(m_data.begin(), m_data.end(), [&counter, &vec](T &elem) {
                elem -= vec.m_data[counter++];
            });
            // clang-format on
        }

        /***********************************************************************************************
         * Multiplies this vector with a value in-place
         *
         * Used by arithmetic operator overloads
         *
         * @tparam U value type
         * @param value value to multiply by
         **********************************************************************************************/
        template <Arithmetic U>
        auto mul(U const &value) -> void
        requires MultiplicableBinary<T, U>
        {
            // clang-format off
            std::for_each(m_data.begin(), m_data.end(), [&value](T &elem) {
                elem *= value;
            });
            // clang-format on
        }

        /***********************************************************************************************
         * Multiplies this vector with another vector in-place
         *
         * Used by arithmetic operator overloads
         *
         * @tparam U other vector value type
         * @param vec vector to multiply with
         **********************************************************************************************/
        template <Arithmetic U>
        auto mul(Vec<U, Count> const &vec) -> void
        requires MultiplicableBinary<T, U>
        {
            usize counter = { 0U };

            // clang-format off
            std::for_each(m_data.begin(), m_data.end(), [&counter, &vec](T &elem) {
                elem *= vec.m_data[counter++];
            });
            // clang-format on
        }

        /***********************************************************************************************
         * Divides this vector by a value in-place
         *
         * Used by arithmetic operator overloads
         *
         * @tparam U value type
         * @param value value to add
         **********************************************************************************************/
        template <Arithmetic U>
        auto div(U const &value) -> void
        requires DivisibleBinary<T, U>
        {
            // clang-format off
            std::for_each(m_data.begin(), m_data.end(), [&value](T &elem) {
                elem /= value;
            });
            // clang-format on
        }

        /***********************************************************************************************
         * Divides this vector by another vector in-place
         *
         * Used by arithmetic operator overloads
         *
         * @tparam U other vector value type
         * @param vec vector to divide by
         **********************************************************************************************/
        template <Arithmetic U>
        auto div(Vec<U, Count> const &vec) -> void
        requires DivisibleBinary<T, U>
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
        // Associated data ========================================================================== //
        // ========================================================================================== //

        /** Underlying data of the vector */
        std::array<T, Count> m_data;

        // ========================================================================================== //
        // Constructors ============================================================================= //
        // ========================================================================================== //

        Vec() = default;

        /***********************************************************************************************
         * Delegates the vectors construction to the arrays constructor
         * and forwards all arguments
         *
         * @tparam Args templated argument list
         * @param args packed arguments delegated to `std::array`
         **********************************************************************************************/
        template <typename... Args>
        constexpr explicit Vec(Args... args): m_data { { args... } }
        {
            constexpr usize dims = sizeof...(args);
            static_assert(dims == Count);
        }

        // ========================================================================================== //
        // Mutators and transformer methods ========================================================= //
        // ========================================================================================== //

        /***********************************************************************************************
         * Pops an element from the vector
         *
         * @return mutated vector with one less element
         **********************************************************************************************/
        auto pop() -> Vec<T, Count - 1>
        {
            Vec<T, Count - 1> result = {};

            std::copy_n(m_data.begin(), Count - 1, result.m_data.begin());

            return (result);
        }

        /***********************************************************************************************
         * Pushes an element into the vector
         *
         * @param value value to add
         * @return mutated vector with one more element
         **********************************************************************************************/
        auto push(T const &value) -> Vec<T, Count + 1>
        {
            Vec<T, Count + 1> result = {};

            std::copy_n(m_data.begin(), Count + 1, result.m_data.begin());
            result.m_data[Count] = value;

            return (result);
        }

        /***********************************************************************************************
         * Return a `const-ref` to an element at a given index in the vector
         *
         * @tparam Index index of the element
         * @return `const-ref` of element at `index`
         **********************************************************************************************/
        template <usize Index>
        auto constexpr get_value() const -> T const &
        {
            return (m_data[Index]);
        }

        /***********************************************************************************************
         * Return an element at a given index in the vector
         *
         * @tparam Index index of the element
         * @return element at `index`
         **********************************************************************************************/
        template <usize Index>
        auto get_value() -> T &
        {
            return (m_data[Index]);
        }

        /***********************************************************************************************
         * Return a `const-ref` to an element at a given index in the vector
         *
         * Uses the template version internally
         *
         * @param index index of the element
         * @return `const-ref` of element at `index`
         **********************************************************************************************/
        auto constexpr get_value(usize index) const -> T const &
        {
            return (this->get_value<index>());
        }

        /***********************************************************************************************
         * Return an element at a given index in the vector
         *
         * Uses the template version internally
         *
         * @param index index of the element
         * @return element at `index`
         **********************************************************************************************/
        auto get_value(usize index) -> T &
        {
            return (this->get_value<index>());
        }

        /***********************************************************************************************
         * Set an element in the vector using
         * the same method provided by `std::array`
         *
         * @tparam Args templated argument list
         * @param args packed arguments delegated to `std::array`
         **********************************************************************************************/
        template <typename... Args>
        auto set(Args... args) -> void
        {
            constexpr usize dims = sizeof...(args);
            static_assert(dims == Count);

            m_data = { { args... } };
        }

        /***********************************************************************************************
         * Return the computed length of the array
         *
         * @return computed length
         **********************************************************************************************/
        auto length()
        requires AddableUnary<T> && MultiplicableUnary<T>
        {
            auto square = m_data;

            // clang-format off
            std::for_each(square.begin(), square.end(), [](T &elem) {
                elem = (elem * elem);
            });
            // clang-format on

            auto sum = std::accumulate(square.begin(), square.end(), static_cast<T>(0U));

            return (std::sqrt(sum));
        }

        /** Normalize the vector in-place */
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

        /** Returns what would be the `x` component of a vector */
        auto x() -> T &
        {
            static_assert(1 <= Count, "Invalid number of arguments for vector type");
            return (this->get_value<0>());
        }

        auto constexpr x() const -> T const &
        {
            static_assert(1 <= Count, "Invalid number of arguments for vector type");
            return (this->get_value<0>());
        }

        /** Returns what would be the `y` component of a vector */
        auto y() -> T &
        {
            static_assert(2 <= Count, "Invalid number of arguments for vector type");
            return (this->get_value<1>());
        }

        auto constexpr y() const -> T const &
        {
            static_assert(2 <= Count, "Invalid number of arguments for vector type");
            return (this->get_value<1>());
        }

        /** Returns what would be the `z` component of a vector */
        auto z() -> T &
        {
            static_assert(3 <= Count, "Invalid number of arguments for vector type");
            return (this->get_value<2>());
        }

        auto constexpr z() const -> T const &
        {
            static_assert(3 <= Count, "Invalid number of arguments for vector type");
            return (this->get_value<2>());
        }

        /** Returns what would be the `w` component of a vector */
        auto w() -> T &
        {
            static_assert(4 <= Count, "Invalid number of arguments for vector type");
            return (this->get_value<3>());
        }

        auto constexpr w() const -> T const &
        {
            static_assert(4 <= Count, "Invalid number of arguments for vector type");
            return (this->get_value<3>());
        }

        /** Returns what would be the `r` component of a vector */
        auto r() -> T &
        {
            return (this->x());
        }

        auto constexpr r() const -> T const &
        {
            return (this->x());
        }

        /** Returns what would be the `g` component of a vector */
        auto g() -> T &
        {
            return (this->y());
        }


        auto constexpr g() const -> T const &
        {
            return (this->y());
        }

        /** Returns what would be the `b` component of a vector */
        auto b() -> T &
        {
            return (this->z());
        }

        auto constexpr b() const -> T const &
        {
            return (this->z());
        }

        /** Returns what would be the `a` component of a vector */
        auto a() -> T &
        {
            return (this->w());
        }

        auto constexpr a() const -> T const &
        {
            return (this->w());
        }


        // ========================================================================================== //
        // Operator overloads ======================================================================= //
        // ========================================================================================== //

        // 1. Addition ============================================================================== //
        // ========================================================================================== //

        /***********************************************************************************************
         * Adds the `other` vector in-place
         *
         * @tparam U other vector value type
         * @param other vector to add
         * @return mutated instance
         **********************************************************************************************/
        template <Arithmetic U>
        auto operator+=(Vec<U, Count> const &other)
        {
            this->add(other);

            return (*this);
        }

        /***********************************************************************************************
         * Adds `other` value in-place
         *
         * @tparam U value type
         * @param other value to add
         * @return mutated instance
         **********************************************************************************************/
        template <Arithmetic U>
        auto operator+=(U const &other)
        {
            this->add(other);

            return (*this);
        }

        /***********************************************************************************************
         * Add `lhs` and `rhs` vectors and return the result
         *
         * @tparam U other vector value type
         * @param lhs first operand
         * @param rhs second operand
         * @return result of addition
         **********************************************************************************************/
        template <Arithmetic U>
        friend inline auto operator+(Vec<T, Count> lhs, Vec<U, Count> const &rhs)
        {
            lhs += rhs;

            return (lhs);
        }

        /***********************************************************************************************
         * Add `lhs` vector and `rhs` value and return the result
         *
         * @tparam U value type
         * @param lhs first operand
         * @param rhs second operand
         * @return result of addition
         **********************************************************************************************/
        template <Arithmetic U>
        friend inline auto operator+(Vec<T, Count> lhs, U const &rhs)
        {
            lhs += rhs;

            return (lhs);
        }

        // 2. Subtraction =========================================================================== //
        // ========================================================================================== //

        /***********************************************************************************************
         * Subtracts the `other` vector in-place
         *
         * @tparam U other vector value type
         * @param other vector to subtract
         * @return mutated instance
         **********************************************************************************************/
        template <Arithmetic U>
        auto operator-=(Vec<U, Count> const &other)
        {
            this->sub(other);

            return (*this);
        }

        /***********************************************************************************************
         * Subtracts `other` value in-place
         *
         * @tparam U value type
         * @param other value to subtract
         * @return mutated instance
         **********************************************************************************************/
        template <Arithmetic U>
        auto operator-=(U const &other)
        {
            this->sub(other);

            return (*this);
        }

        /***********************************************************************************************
         * Subtract `lhs` and `rhs` vectors and return the result
         *
         * @tparam U other vector value type
         * @param lhs first operand
         * @param rhs second operand
         * @return result of subtraction
         **********************************************************************************************/
        template <Arithmetic U>
        friend inline auto operator-(Vec<T, Count> lhs, Vec<U, Count> const &rhs)
        {
            lhs -= rhs;

            return (lhs);
        }

        /***********************************************************************************************
         * Subtract `lhs` vector and `rhs` value and return the result
         *
         * @tparam U value type
         * @param lhs first operand
         * @param rhs second operand
         * @return result of subtraction
         **********************************************************************************************/
        template <Arithmetic U>
        friend inline auto operator-(Vec<T, Count> lhs, U const &rhs)
        {
            lhs -= rhs;

            return (lhs);
        }

        // 3. Multiplication ======================================================================== //
        // ========================================================================================== //

        /***********************************************************************************************
         * Multiplies with the `other` vector in-place
         *
         * @tparam U other vector value type
         * @param other vector to multiply with
         * @return mutated instance
         **********************************************************************************************/
        template <Arithmetic U>
        auto operator*=(Vec<U, Count> const &other)
        {
            this->mul(other);

            return (*this);
        }

        /***********************************************************************************************
         * Multiplies with the `other` value in-place
         *
         * @tparam U value type
         * @param other value to multiply with
         * @return mutated instance
         **********************************************************************************************/
        template <Arithmetic U>
        auto operator*=(U const &other)
        {
            this->mul(other);

            return (*this);
        }

        /***********************************************************************************************
         * Mutiply `lhs` and `rhs` vectors and return the result
         *
         * @tparam U other vector value type
         * @param lhs first operand
         * @param rhs second operand
         * @return result of multiplication
         **********************************************************************************************/
        template <Arithmetic U>
        friend inline auto operator*(Vec<T, Count> lhs, Vec<U, Count> const &rhs)
        {
            lhs *= rhs;

            return (lhs);
        }

        /***********************************************************************************************
         * Multiply `lhs` vector and `rhs` value and return the result
         *
         * @tparam U value type
         * @param lhs first operand
         * @param rhs second operand
         * @return result of multiplication
         **********************************************************************************************/
        template <Arithmetic U>
        friend inline auto operator*(Vec<T, Count> lhs, U const &rhs)
        {
            lhs *= rhs;

            return (lhs);
        }

        // 3. Division ============================================================================== //
        // ========================================================================================== //

        /***********************************************************************************************
         * Divides by the `other` vector in-place
         *
         * @tparam U other vector value type
         * @param other vector to divide by
         * @return mutated instance
         **********************************************************************************************/
        template <Arithmetic U>
        auto operator/=(Vec<U, Count> const &other)
        {
            this->div(other);

            return (*this);
        }

        /***********************************************************************************************
         * Divides by `other` value in-place
         *
         * @tparam U value type
         * @param other value to divide by
         * @return mutated instance
         **********************************************************************************************/
        template <Arithmetic U>
        auto operator/=(U const &other)
        {
            this->div(other);

            return (*this);
        }

        /***********************************************************************************************
         * Divide `lhs` and `rhs` vectors and return the result
         *
         * @tparam U other vector value type
         * @param lhs first operand
         * @param rhs second operand
         * @return result of division
         **********************************************************************************************/
        template <Arithmetic U>
        friend inline auto operator/(Vec<T, Count> lhs, Vec<U, Count> const &rhs)
        {
            lhs /= rhs;

            return (lhs);
        }

        /***********************************************************************************************
         * Divide `lhs` vector and `rhs` value and return the result
         *
         * @tparam U value type
         * @param lhs first operand
         * @param rhs second operand
         * @return result of division
         **********************************************************************************************/
        template <Arithmetic U>
        friend inline auto operator/(Vec<T, Count> lhs, U const &rhs)
        {
            lhs /= rhs;

            return (lhs);
        }

        // ========================================================================================== //
        // Debugging utilities ====================================================================== //
        // ========================================================================================== //

        /***********************************************************************************************
         * Converts the vector into a string
         *
         * @return vector as string
         **********************************************************************************************/
        [[nodiscard]] auto to_string() const -> std::string
        {
            // clang-format off
            auto format = fmt::format(
                "[Vec<{}, {}>]: \n",
                demangle::type_name<T>(), 
                Count
            );
            // clang-format on

            for (usize i = 0; i < Count; ++i) {
                // 'A' for 0
                // 'B' for 1 etc.
                char var_name = static_cast<char>((i + static_cast<usize>('A')));

                // Format into the previous string.
                fmt::format_to(
                    std::back_inserter<std::string>(format), "{:>4}{}: {}\n", ' ', var_name, m_data[i]);
            }

            return (format);
        };

        /***********************************************************************************************
         * Allows writing the vector into an output stream
         *
         * @param os stream to write to
         * @param vec vector to write
         * @return reference to used stream
         **********************************************************************************************/
        friend inline auto operator<<(std::ostream &ostream, Vec<T, Count> const &vec) -> std::ostream &
        {
            return (ostream << vec.to_string());
        }
    };

    /***************************************************************************************************
     * Type alias for creating 2D vectors
     *
     * @tparam T value type
     **************************************************************************************************/
    template <Arithmetic T>
    using Vec2 = Vec<T, 2>;

    /***************************************************************************************************
     * Type alias for creating 3D vectors
     *
     * @tparam T value type
     **************************************************************************************************/
    template <Arithmetic T>
    using Vec3 = Vec<T, 3>;

    /***************************************************************************************************
     * Type alias for creating 4D vectors
     *
     * @tparam T value type
     **************************************************************************************************/
    template <Arithmetic T>
    using Vec4 = Vec<T, 4>;
}  // namespace util::math

template <Arithmetic T, usize Count>
struct fmt::formatter<util::math::Vec<T, Count>>: formatter<std::string>
{
    auto format(util::math::Vec<T, Count> const &vec, format_context &ctx) const -> format_context::iterator
    {
        return formatter<std::string>::format(vec.to_string(), ctx);
    }
};

using namespace util::math;
