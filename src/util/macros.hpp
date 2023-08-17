/** @file macros.hpp */

#pragma once

// module includes
#include "traits.hpp"
#include "types.hpp"

// c++ includes
#include <exception>
#include <functional>

#if !defined(CONCAT_IMPL)

/****************************************************************************************
 * Concatenates stringified `s1` and `s2`
 *
 * @param s1 first concat param
 * @param s2 second concat param
 * @return concatenated string
 ***************************************************************************************/
#define CONCAT_IMPL(s1, s2) s1##s2
#endif

#if !defined(CONCAT)

/****************************************************************************************
 * Expands `s1` and `s2` and then concatenates them both.
 *
 * @param s1 first concat param
 * @param s2 second concat param
 * @return expanded and concatenated string
 ***************************************************************************************/
#define CONCAT(s1, s2) CONCAT_IMPL(s1, s2)
#endif

#if defined(__COUNTER__) && !defined(ANON_VAR)
/****************************************************************************************
 * Generates an anonymous variable with a unique name based on `str` and
 * the builtin `__COUNTER__`.
 *
 * @param str prefix of variable
 * @returns i.e. for `str = SCOPE` generates `SCOPE1234` if `__COUNTER__ = 1234`
 ***************************************************************************************/
#define ANON_VAR(str) CONCAT(str, __COUNTER__)
#elif !defined(__COUNTER__) && !defined(ANON_VAR)

/****************************************************************************************
 * Generates an anonymous variable with a unique name based on `str` and
 * the line at which it was invoked.
 *
 * @param str prefix of variable
 * @returns i.e. for `str = SCOPE` generates `SCOPE34` if `__LINE__ = 34`
 ***************************************************************************************/
#define ANON_VAR(str) CONCAT(str, __LINE__)
#endif

#if !defined(SCOPE_EXIT)

/****************************************************************************************
 * Generates a lambda whenever a scope is left.
 ***************************************************************************************/
#define SCOPE_EXIT auto ANON_VAR(SCOPE_EXIT_STATE) = ::util::macros::MakeScopeExit() + [&]()
#endif

#if !defined(SCOPE_FAIL)

/****************************************************************************************
 * Generates a scope in the case of failure i.e. exception has been thrown.
 ***************************************************************************************/
#define SCOPE_FAIL auto ANON_VAR(SCOPE_FAIL_STATE) = ::util::macros::MakeScopeFail() + [&]() noexcept
#endif

#if !defined(SCOPE_SUCCESS)

/****************************************************************************************
 * Generates a scope in the case of success.
 ***************************************************************************************/
#define SCOPE_SUCCESS auto ANON_VAR(SCOPE_SUCCESS_STATE) = ::util::macros::MakeScopeSuccess + [&]()
#endif

namespace util::macros::details
{
    /************************************************************************************
     * Utility counter for how many uncaught exceptions have been thrown.
     ***********************************************************************************/
    class UncaughtExceptionCounter
    {
    private:
        /** Amount of exceptions thrown. */
        i32 m_count;

        /********************************************************************************
         * Access to the internal `m_count`.
         *
         * @return amount of exceptions thrown
         *******************************************************************************/
        [[nodiscard]] inline auto get_count() const noexcept -> i32
        {
            return (m_count);
        }

    public:
        UncaughtExceptionCounter() : m_count(std::uncaught_exceptions())
        {
        }

        /********************************************************************************
         * Updates internal counter if a new uncaught exception has been thrown.
         *
         * @return true if new exception is thrown and uncaught
         *******************************************************************************/
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
