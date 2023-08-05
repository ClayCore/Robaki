/** @file math.hpp
 * Various math utilities
 */

#pragma once

// module includes
#include "traits.hpp"
#include "types.hpp"

namespace util::math
{
    template <typename T>
    requires std::is_arithmetic_v<T>
    struct Vec2
    {
        T x;
        T y;
    };

    template <typename T>
    requires std::is_arithmetic_v<T>
    struct Vec3
    {
        T x;
        T y;
        T z;
    };
}  // namespace util::math

using namespace util::math;
