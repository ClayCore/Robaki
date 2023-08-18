/** @file types.hpp
 * Useful type aliases
 */

#pragma once

// c includes
#include <cinttypes>
#include <cstddef>
#include <cstdint>

// c++ includes
#include <memory>
#include <vector>

namespace util::types
{
    using u8  = std::uint_fast8_t;
    using u16 = std::uint_fast16_t;
    using u32 = std::uint_fast32_t;
    using u64 = std::uint_fast64_t;

    using i8  = std::int_fast8_t;
    using i16 = std::int_fast16_t;
    using i32 = std::int_fast32_t;
    using i64 = std::int_fast64_t;

    using usize = std::uintptr_t;
    using isize = std::ptrdiff_t;

    using f32  = float;
    using f64  = double;
    using f128 = long double;

    /*********************************************************************************************************
     * Utility type alias used to refer to unique pointers
     *
     * @tparam T pointee
     ********************************************************************************************************/
    template <typename T>
    using Scope = std::unique_ptr<T>;

    /*********************************************************************************************************
     * Utility function to create unique pointers
     *
     * @tparam T pointee
     * @tparam Args packed arguments forwarded to `std::make_unique`
     * @param args unpacked arguments
     ********************************************************************************************************/
    template <typename T, typename... Args>
    constexpr auto CreateScope(Args &&...args) -> Scope<T>
    {
        return (std::make_unique<T>(std::forward<Args>(args)...));
    }

    /*********************************************************************************************************
     * Utility type alias used to refer to shared pointers
     *
     * @tparam T pointee
     ********************************************************************************************************/
    template <typename T>
    using Ref = std::shared_ptr<T>;

    /*********************************************************************************************************
     * Utility function to create shared pointers
     *
     * @tparam T pointee
     * @tparam Args packed arguments forwarded to `std::make_shared`
     * @param args unpacked arguments
     ********************************************************************************************************/
    template <typename T, typename... Args>
    constexpr auto CreateRef(Args &&...args) -> Ref<T>
    {
        return (std::make_shared<T>(std::forward<Args>(args)...));
    }

    /** CRTP NonCopyable mixin used by various traits and types */
    template <class T>
    struct NonCopyable
    {
    protected:
        NonCopyable()  = default;
        ~NonCopyable() = default;

    public:
        NonCopyable(NonCopyable &&)                 = delete;
        NonCopyable &operator=(const NonCopyable &) = delete;
        NonCopyable &operator=(NonCopyable &&)      = delete;
        NonCopyable(NonCopyable const &)            = delete;

        auto operator=(T const &) -> T & = delete;
    };

    /*********************************************************************************************************
     * Used by various managers.
     ********************************************************************************************************/
    template <class Derived>
    class Singleton : private NonCopyable<Derived>
    {
    private:
        /** Thin wrapper to invoke ctors defined in the `Derived` type */
        struct instance_t : public Derived
        {
            instance_t() : Derived()
            {
            }
        };

        /** Global, mutable and shared instance to any object that implements `Singleton` */
        static inline Derived *m_instance = nullptr;

    public:
        // ================================================================================================ //
        // Accessor methods =============================================================================== //
        // ================================================================================================ //

        /*****************************************************************************************************
         * Provides an instance (or creates one, if it doesn't yet exist)
         *
         * @return instance of concrete derivative of `Singleton`
         ****************************************************************************************************/
        static auto instance() -> Derived &
        {
            if (!m_instance) {
                m_instance = new instance_t;
            }

            return (*m_instance);
        }
    };

    /** Describes a circular queue view of `std::vector` */
    template <class T>
    class CircularQueue
    {
    private:
        usize m_max_items = { 0U };
        usize m_overrun   = { 0U };

        std::vector<T>::size_type m_head = {};
        std::vector<T>::size_type m_tail = {};

        std::vector<T> m_vector;

        auto copy_moveable(CircularQueue &&other) noexcept
        {
            m_vector = std::move(other.m_vector);

            m_max_items = other.m_max_items;
            m_head      = other.m_head;
            m_tail      = other.m_tail;
            m_overrun   = other.m_overrun;

            // puts other in disabled but still valid state
            other.m_max_items = { 0 };
            other.m_head      = { 0 };
            other.m_tail      = { 0 };
            other.m_overrun   = { 0 };
        }

    public:
        using value_type = T;

        CircularQueue()                                          = default;
        CircularQueue(CircularQueue const &)                     = default;
        auto operator=(CircularQueue const &) -> CircularQueue & = default;

        CircularQueue(CircularQueue &&other) noexcept
        {
            this->copy_moveable(std::move(other));
        }

        auto operator=(CircularQueue &&other) noexcept -> CircularQueue &
        {
            this->copy_moveable(std::move(other));

            return (*this);
        }

        explicit CircularQueue(usize max_items) noexcept : m_max_items(max_items + 1), m_vector(m_max_items)
        {
        }

        template <usize Index>
        [[nodiscard]] constexpr auto at() const -> T const &
        {
            static_assert(Index < this->size());

            return (m_vector[(m_head + Index) % m_max_items]);
        }

        [[nodiscard]] constexpr auto empty() const noexcept -> bool
        {
            return (m_tail == m_head);
        }

        [[nodiscard]] constexpr auto full() const noexcept -> bool
        {
            if constexpr (m_max_items > 0) {
                return (((m_tail + 1) % m_max_items) == m_head);
            } else {
                return (false);
            }
        }

        [[nodiscard]] auto front() const noexcept -> T const &
        {
            return (m_vector[m_head]);
        }

        [[nodiscard]] auto front() noexcept -> T &
        {
            return (m_vector[m_head]);
        }

        [[nodiscard]] constexpr auto size() const noexcept -> usize
        {
            if constexpr (m_tail >= m_head) {
                return (m_tail - m_head);
            } else {
                return (m_max_items - (m_head - m_tail));
            }
        }

        [[nodiscard]] constexpr auto overrun() const noexcept -> usize
        {
            return (m_overrun);
        }

        auto pop_front() -> void
        {
            m_head = (m_head + 1) % m_max_items;
        }

        auto push_back(T &&item) -> void
        {
            if (m_max_items > 0) {
                m_vector[m_tail] = std::move(item);
                m_tail           = (m_tail + 1) % m_max_items;

                if (m_tail == m_head) {
                    m_head = (m_head + 1) % m_max_items;
                    m_overrun += 1;
                }
            }
        }

        auto reset_overrun() -> void
        {
            m_overrun = 0;
        }
    };
}  // namespace util::types

using namespace util::types;
