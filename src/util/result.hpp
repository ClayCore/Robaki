/*******************************************************************************************************
 * @file result.hpp
 * `Result<T, E>` type. Used for error handling.
 *
 * Implementation in according to Andrei Alexandrescu's talks @ cppcon
 ******************************************************************************************************/

#pragma once

// module includes
#include "traits.hpp"
#include "types.hpp"


namespace util::types
{
    /***************************************************************************************************
     * Thin wrapper around whatever `E` type.
     *
     * Allows us to tell the difference between 2 identical types used for `T, E` in
     * `Result<T, E>`.
     *
     * @tparam E type to wrap
     **************************************************************************************************/
    template <class E>
    struct Unexpected
    {
        E error;

        Unexpected() = default;
        explicit Unexpected(E const &err): error { err }
        {
        }
        explicit Unexpected(E &&err): error { std::move(err) }
        {
        }
    };

    /***************************************************************************************************
     * Result type implementation
     *
     * @tparam T
     **************************************************************************************************/
    template <class T, class E>
    class Result
    {
    private:
        union
        {
            /** Stores the value of `Result` */
            T m_ok;
            /** Stores the error of `Result` */
            E m_err;
        };

        /** Optimistically initialized to true */
        bool m_is_ok { true };

    public:
        // ========================================================================================== //
        // Ctors and Dtors ========================================================================== //
        // ========================================================================================== //

        /** Optimistically allocate `T` by default*/
        Result()
        {
            new (&m_ok) T {};
        }

        /** Allocates `T` */
        explicit Result(T const &ok)
        {
            new (&m_ok) T { ok };
        }

        /** Allocates `E` and alters `m_is_ok` */
        explicit Result(Unexpected<E> const &err): m_is_ok { false }
        {
            new (&m_err) E { err.error };
        }

        // FIXME(claymore): Might shadow copy and move constructors
        template <class U = T>
        explicit Result(U &&rhs)
        {
            new (&m_ok) T { std::forward<U>(rhs) };
        }

        Result(Result const &rhs) noexcept: m_is_ok { rhs.m_is_ok }
        {
            if (m_is_ok) {
                new (&m_ok) T { rhs.m_ok };
            } else {
                new (&m_err) E { rhs.m_err };
            }
        }

        Result(Result &&rhs) noexcept: m_is_ok { rhs.m_is_ok }
        {
            if (m_is_ok) {
                new (&m_ok) T { std::move(rhs.m_ok) };
            } else {
                new (&m_err) E { std::move(rhs.m_err) };
            }
        }

        // ========================================================================================== //
        // Operator overloads ======================================================================= //
        // ========================================================================================== //

        /***********************************************************************************************
         * Unsafe dereference of the value
         *
         * @throws E error-object
         * @return ref-value
         **********************************************************************************************/
        auto operator*() -> T &
        {
            if (!m_is_ok) {
                throw m_err;
            }
            return (m_ok);
        }

        /** @overload */
        auto operator*() const -> T const &
        {
            if (!m_is_ok) {
                throw m_err;
            }
            return (m_ok);
        }

        /***********************************************************************************************
         * Acquire const pointer to the value
         *
         * @throws E error object
         * @return access into value
         **********************************************************************************************/
        auto operator->() const -> T const *
        {
            return (&**this);
        }

        /** @overload */
        auto operator->() -> T *
        {
            return (&**this);
        }

        // ========================================================================================== //
        // Accessor methods ========================================================================= //
        // ========================================================================================== //

        /***********************************************************************************************
         * Fetch error value, only valid if an error's been set
         *
         * @return ref error
         **********************************************************************************************/
        auto error() -> E &
        {
            static_assert(!m_is_ok);

            return (m_err);
        }

        /** @overload */
        auto error() const -> E const &
        {
            static_assert(!m_is_ok);

            return (m_err);
        }

        /**
         * Checks if there's a value present
         *
         * @return true if value is present
         */
        [[nodiscard]] auto constexpr has_value() const noexcept -> bool
        {
            return (m_is_ok);
        }

        /**
         * Explicit conversion to bool using the `has_value` function
         *
         * @see has_value
         * @returns true if value is present
         */
        explicit constexpr operator bool() const noexcept
        {
            return (m_is_ok);
        }

        /**
         * Acquires value. Valid when there's no error present
         *
         * @throws E error object
         * @returns value
         */
        auto value() -> T &
        {
            if (!m_is_ok) {
                throw m_err;
            }

            return (m_is_ok);
        }

        /** @overload */
        auto value() const -> T const &
        {
            if (!m_is_ok) {
                throw m_err;
            }

            return (m_is_ok);
        }

        // ========================================================================================== //
        // Transformation methods =================================================================== //
        // ========================================================================================== //

        /**
         * Returns internal value if present, otherwise returns the parameter
         *
         * @throws E error object
         * @returns either internal value or parameter
         */
        template <class U>
        auto value_or(U &&value) -> T
        {
            if (m_is_ok) {
                return (this->value());
            }

            return T { std::forward<U>(value) };
        }

        /** @overload */
        template <class U>
        auto value_or(U &&value) const -> T
        {
            if (m_is_ok) {
                return (this->value());
            }

            return T { std::forward<U>(value) };
        }
    };
}  // namespace util::types
