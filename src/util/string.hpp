/** @file string.hpp */

#pragma once

// module includes
#include "traits.hpp"
#include "types.hpp"


// c includes
#include <cctype>

// c++ includes
#include <algorithm>
#include <string>


namespace util
{
    inline auto to_lower(std::string const &str) -> std::string
    {
        auto result{ str };

        // clang-format off
        std::transform(str.cbegin(), str.cend(), result.begin(), [](u8 chr) {
            return (std::tolower(chr));
        });
        // clang-format on

        return (result);
    }
}  // namespace util
