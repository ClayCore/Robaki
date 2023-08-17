/** @file macros.hpp */

#pragma once

// module includes
#include "traits.hpp"
#include "types.hpp"

// c++ includes
#include <exception>
#include <functional>

#if !defined(CONCAT_IMPL)
#define CONCAT_IMPL(s1, s2) s1##s2
#endif

#if !defined(CONCAT)
#define CONCAT(s1, s2) CONCAT_IMPL(s1, s2)
#endif

#if defined(__COUNTER__) && !defined(ANON_VAR)
#define ANON_VAR(str) CONCAT(str, __COUNTER__)
#elif !defined(__COUNTER__) && !defined(ANON_VAR)
#define ANON_VAR(str) CONCAT(str, __LINE__)
#endif

#if !defined(SCOPE_EXIT)
#define SCOPE_EXIT auto ANON_VAR(SCOPE_EXIT_STATE) = ::util::macros::MakeScopeExit() + [&]()
#endif

#if !defined(SCOPE_FAIL)
#define SCOPE_FAIL auto ANON_VAR(SCOPE_FAIL_STATE) = ::util::macros::MakeScopeFail() + [&]() noexcept
#endif

#if !defined(SCOPE_SUCCESS)
#define SCOPE_SUCCESS auto ANON_VAR(SCOPE_SUCCESS_STATE) = ::util::macros::MakeScopeSuccess + [&]()
#endif

namespace util::macros::details
{
    class UncaughtExceptionCounter
    {
    private:
        i32 m_count;

        [[nodiscard]] inline auto get_count() const noexcept -> i32
        {
            return (m_count);
        }

    public:
        UncaughtExceptionCounter() : m_count(std::uncaught_exceptions())
        {
        }

        auto update() noexcept -> bool
        {
            return (std::uncaught_exceptions() > this->get_count());
        }
    };
}  // namespace util::macros::details

namespace util::macros
{
    template <class F, bool IsFail>
    class ScopeGuard : private NonCopyable<ScopeGuard<F, IsFail>>
    {
    private:
        F m_on_exit;
        details::UncaughtExceptionCounter m_counter;

    public:
        ScopeGuard(const ScopeGuard &)            = delete;
        ScopeGuard(ScopeGuard &&)                 = delete;
        ScopeGuard &operator=(const ScopeGuard &) = delete;
        ScopeGuard &operator=(ScopeGuard &&)      = delete;

        explicit ScopeGuard(F &&on_exit) : m_on_exit(std::move(on_exit))
        {
        }
        ~ScopeGuard() noexcept(IsFail)
        {
            if (IsFail == m_counter.update()) {
                m_on_exit();
            }
        }
    };

    template <class F>
    class ScopeExit : private NonCopyable<ScopeExit<F>>
    {
    private:
        F m_on_exit;

    public:
        ScopeExit(const ScopeExit &)            = delete;
        ScopeExit &operator=(const ScopeExit &) = delete;
        ScopeExit &operator=(ScopeExit &&)      = delete;

        explicit ScopeExit(F &&on_exit) : m_on_exit(std::move(on_exit))
        {
        }
        ScopeExit(ScopeExit &&other) noexcept : m_on_exit(std::move(other.m_on_exit))
        {
        }
        ~ScopeExit()
        {
            m_on_exit();
        }
    };

    struct MakeScopeExit
    {
        template <class F>
        auto operator+=(F &&on_exit) -> ScopeExit<F>
        {
            return (ScopeExit<F>{ std::forward<F>(on_exit) });
        }
    };

    struct MakeScopeFail
    {
        template <class F>
        auto operator+=(F &&on_exit) -> ScopeGuard<F, true>
        {
            return (ScopeGuard<F, true>{ std::forward<F>(on_exit) });
        }
    };

    struct MakeScopeSuccess
    {
        template <class F>
        auto operator+=(F &&on_exit) -> ScopeGuard<F, false>
        {
            return (ScopeGuard<F, false>{ std::forward<F>(on_exit) });
        }
    };
}  // namespace util::macros
