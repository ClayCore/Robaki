#pragma once

// module includes
#include "message.hpp"
#include "util/util.hpp"

namespace logging
{
    class MessageBuffer : public Message
    {
    private:
        std::string m_buffer = {};

        auto update() -> void;
    };
}  // namespace logging
