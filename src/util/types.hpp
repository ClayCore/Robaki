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

namespace util::types
{
    using u8  = std::uint8_t;
    using u16 = std::uint16_t;
    using u32 = std::uint32_t;
    using u64 = std::uint64_t;

    using i8  = std::int8_t;
    using i16 = std::int16_t;
    using i32 = std::int32_t;
    using i64 = std::int64_t;

    using usize = std::size_t;
    using isize = std::ptrdiff_t;

    using f32  = float;
    using f64  = double;
    using f128 = long double;

    /**
     * Utility type alias used to refer to unique pointers
     *
     * @tparam T pointee
     */
    template <typename T>
    using Scope = std::unique_ptr<T>;

    /**
     * Utility function to create unique pointers
     *
     * @tparam T pointee
     * @tparam Args packed arguments forwarded to `std::make_unique`
     * @param args unpacked arguments
     */
    template <typename T, typename... Args>
    constexpr auto CreateScope(Args &&...args) -> Scope<T>
    {
        return (std::make_unique<T>(std::forward<Args>(args)...));
    }

    /**
     * Utility type alias used to refer to shared pointers
     *
     * @tparam T pointee
     */
    template <typename T>
    using Ref = std::shared_ptr<T>;

    /**
     * Utility function to create shared pointers
     *
     * @tparam T pointee
     * @tparam Args packed arguments forwarded to `std::make_shared`
     * @param args unpacked arguments
     */
    template <typename T, typename... Args>
    constexpr auto CreateRef(Args &&...args) -> Ref<T>
    {
        return (std::make_shared<T>(std::forward<Args>(args)...));
    }

    /**
     * Used by any managing classes. Only
     */
    template <typename Derived>
    class Singleton
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

    protected:
        /** Default ctor */
        Singleton() = default;

    public:
        // ================================================================================================ //
        // Deleted ctors and dtors ======================================================================== //
        // ================================================================================================ //

        Singleton(const Singleton &)                     = delete;
        Singleton(Singleton &&)                          = delete;
        auto operator=(Singleton const &) -> Singleton & = delete;
        auto operator=(Singleton &&) -> Singleton      & = delete;

        // ================================================================================================ //
        // Accessor methods =============================================================================== //
        // ================================================================================================ //

        /**
         * Provides an instance (or creates one, if it doesn't yet exist)
         *
         * @return instance of concrete derivative of `Singleton`
         */
        static auto instance() -> Derived &
        {
            if (!m_instance) {
                m_instance = new instance_t;
            }

            return (*m_instance);
        }
    };
}  // namespace util::types

using namespace util::types;
