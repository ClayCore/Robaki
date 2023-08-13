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
#include <string_view>


namespace util::string
{
    namespace details
    {
        constexpr const u32 DEFAULT_KEY_HASH_STR = (5381U);
    }

    constexpr auto hash_str(std::string_view const &str) -> u32
    {
        auto hash = details::DEFAULT_KEY_HASH_STR;
        for (u8 chr : str) {
            hash = ((hash << ((2U << 1U) ^ 1U)) + hash) ^ chr;
        }

        return (hash);
    }

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
}  // namespace util::string
