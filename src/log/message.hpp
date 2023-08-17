/** @file message.hpp */

#pragma once

// module includes
#include "location.hpp"
#include "util/util.hpp"

namespace logging
{
    struct Message
    {
        struct MessageProps
        {
            util::time::timepoint time = {};
            std::string_view name      = {};
            std::string_view msg       = {};
            details::Level level       = {};
            Location loc               = {};
        };

        MessageProps props = {};
        usize thread       = { 0U };

        usize mutable color_start = { 0U };
        usize mutable color_end   = { 0U };

        std::string_view payload = {};

        Message()                = default;
        Message(Message const &) = default;

        auto operator=(Message const &) -> Message & = default;

        explicit Message(MessageProps options)
        {
            props.time  = options.time;
            props.name  = options.name;
            props.msg   = options.msg;
            props.level = options.level;
            props.loc   = options.loc;
        }
    };
}  // namespace logging
