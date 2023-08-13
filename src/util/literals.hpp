/** @file literals.hpp */

#pragma once

// module includes
#include "string.hpp"
#include "traits.hpp"
#include "types.hpp"
#include "util/string.hpp"


// c++ includes

namespace util::literals
{
    constexpr auto operator"" _sh(char const *str, usize len) -> u32
    {
        return (util::string::hash_str(std::string_view{ str, len }));
    }
}  // namespace util::literals
