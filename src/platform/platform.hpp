/** @file platform.hpp */

#pragma once

// module includes
#include "input.hpp"
#include "util/util.hpp"
#include "window.hpp"

// c++ includes
#include <unordered_map>

namespace platform
{
    struct Platform
    {
        Scope<Window> m_window;
        std::unordered_map<std::string, Scope<Input>> m_inputs;

        Platform() = default;

        auto update() -> void;
        auto tick() -> void;

        template <typename InputType>
        inline auto get_input(std::string const &name = "__USE_TYPENAME") -> InputType &
        {
            std::string raw = { name };

            if (name == "__USE_TYPENAME") {
                raw = util::demangle::type_name<InputType>();
            }

            return (*dynamic_cast<InputType *>(m_inputs[util::to_lower(raw)].get()));
        }
    };
}  // namespace platform
