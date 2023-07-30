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

    template <class Derived>
    class Singleton
    {
    private:
        struct instance_t : public Derived
        {
            instance_t() : Derived()
            {
            }
        };

        static inline Derived *m_instance = nullptr;

    protected:
        Singleton()
        {
        }

    public:
        auto operator=(Singleton const &) -> Singleton & = delete;
        auto operator=(Singleton &&) -> Singleton      & = delete;

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
