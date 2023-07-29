/** @file demangle.hpp
 * Used for demangling type names when printing them
 */

#pragma once

// c++ includes
#include <string_view>

// module includes
#include "types.hpp"

namespace util::demangle
{
    template <typename T>
    constexpr auto type_name() -> std::string_view
    {
        std::string_view name, prefix, suffix;

        // clang-format off
        
        #if defined(__CLANG__)
        name   = __PRETTY_FUNCTION__;
        prefix = "std::string_view utils::type_name() [T = ";
        suffix = "; std::string_view = std::basic_string_view<char>]";
        #elif defined(__GNUC__)
        name   = __PRETTY_FUNCTION__;
        prefix = "constexpr std::string_view utils::type_name() [with T = ";
        suffix = "; std::string_view = std::basic_string_view<char>]";
        #elif defined(_MSC_VER)
        name   = __FUNCSIG__;
        prefix = "std::string_view __cdecl utils::type_name<";
        suffix = ">(void)";
        #endif

        // clang-format on

        name.remove_prefix(prefix.size());
        name.remove_suffix(suffix.size());

        return (name);
    }
}  // namespace util::demangle
