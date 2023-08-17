/** @file location.hpp */

#pragma once

// module includes
#include "details.hpp"
#include "util/util.hpp"

// c++ includes
#include <string_view>

namespace logging
{
    struct Location
    {
        std::string_view path = { nullptr };
        std::string_view func = { nullptr };
        i32 line              = { 0 };

        constexpr Location() = default;
        constexpr explicit Location(std::string_view const &path, i32 line, std::string_view const &func)
          : path(path), func(func), line(line)
        {
        }

        [[nodiscard]] constexpr auto empty() const -> bool
        {
            return (line == 0);
        }
    };
}  // namespace logging
