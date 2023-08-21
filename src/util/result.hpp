/** @file result.hpp */

#pragma once

// module includes
#include "traits.hpp"
#include "types.hpp"


namespace util::types
{
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

    template <class T, class E>
    class Result
    {
    private:
        union
        {
            T m_ok;
            E m_err;
        };

        bool m_is_ok { true };

    public:
        Result()
        {
            new (&m_ok) T {};
        }

        explicit Result(T const &ok)
        {
            new (&m_ok) T { ok };
        }

        explicit Result(Unexpected<E> const &err): m_is_ok { false }
        {
            new (&m_err) E { err.error };
        }

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

        auto operator*() -> T &
        {
            if (!m_is_ok) {
                throw m_err;
            }
            return (m_ok);
        }

        auto operator*() const -> T const &
        {
            if (!m_is_ok) {
                throw m_err;
            }
            return (m_ok);
        }

        auto operator->() const -> T const *
        {
            return (&**this);
        }

        auto error() -> E &
        {
            static_assert(!m_is_ok);

            return (m_err);
        }

        auto error() const -> E const &
        {
            static_assert(!m_is_ok);

            return (m_err);
        }

        [[nodiscard]] auto constexpr has_value() const noexcept -> bool
        {
            return (m_is_ok);
        }

        explicit constexpr operator bool() const noexcept
        {
            return (m_is_ok);
        }

        auto value() -> T &
        {
            if (!m_is_ok) {
                throw m_err;
            }

            return (m_is_ok);
        }

        auto value() const -> T const &
        {
            if (!m_is_ok) {
                throw m_err;
            }

            return (m_is_ok);
        }

        template <class U>
        auto value_or(U &&value) -> T
        {
            if (m_is_ok) {
                return (this->value());
            }

            return T { std::forward<U>(value) };
        }

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
