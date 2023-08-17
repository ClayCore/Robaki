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
    /** Utility alias for referring to a time point */
    using timepoint = std::chrono::system_clock::time_point;

    /** How many nanoseconds in a second. */
    constexpr u64 const NS_PER_SEC = 1000000000;

    /** How many nanoseconds in a millisecond. */
    constexpr u64 const NS_PER_MS = 1000000;

    /** How many milliseconds in a second. */
    constexpr u64 const MS_PER_SEC = 1000;

    /***************************************************************************************************
     * Acquires timestamp using `std::chrono`
     *
     * @tparam D unit for timestamp
     * @return timestamp
     **************************************************************************************************/
    template <IsDuration D>
    static inline auto get_time() -> u64
    {
        using namespace std::chrono;

        auto const system_time = system_clock::now().time_since_epoch();

        return (duration_cast<D>(system_time)).count();
    }

    /***************************************************************************************************
     * Acquires timestamp in nanoseconds
     *
     * @return timestamp
     **************************************************************************************************/
    static inline auto get_time_ns() -> u64
    {
        return (get_time<std::chrono::nanoseconds>());
    }

    /***************************************************************************************************
     * Acquires timestamp in milliseconds
     *
     * @return timestamp
     **************************************************************************************************/
    static inline auto get_time_ms() -> u64
    {
        return (get_time<std::chrono::milliseconds>());
    }
}  // namespace util::time
