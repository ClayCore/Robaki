/** @file time.hpp
 * Time-related utility constants and functions
 */

#pragma once

// module includes
#include "traits.hpp"
#include "types.hpp"


// c++ includes
#include <chrono>

namespace util::time
{
    constexpr u64 const NS_PER_SEC = 1000000000;
    constexpr u64 const NS_PER_MS  = 1000000;
    constexpr u64 const MS_PER_SEC = 1000;

    template <IsDuration D>
    static inline auto get_time() -> u64
    {
        using namespace std::chrono;

        auto const system_time = system_clock::now().time_since_epoch();

        return (duration_cast<D>(system_time)).count();
    }

    static inline auto get_time_ns() -> u64
    {
        return (get_time<std::chrono::nanoseconds>());
    }

    static inline auto get_time_ms() -> u64
    {
        return (get_time<std::chrono::milliseconds>());
    }
}  // namespace util::time
