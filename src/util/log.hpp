/** @file log.hpp */

#pragma once

// module includes
#include "iterator.hpp"
#include "literals.hpp"
#include "macros.hpp"
#include "string.hpp"
#include "time.hpp"
#include "traits.hpp"
#include "types.hpp"
#include "uuid.hpp"

// c++ includes
#include <atomic>
#include <mutex>
#include <string_view>


namespace util::log::details
{
    enum class Level
    {
        Trace,
        Info,
        Warning,
        Debug,
        Error
    };

    struct Location
    {
        std::string_view m_path = { nullptr };
        std::string_view m_func = { nullptr };
        i32 m_line              = { 0 };

        constexpr Location() = default;
        constexpr explicit Location(std::string_view const &path, i32 line, std::string_view const &func)
          : m_path(path), m_func(func), m_line(line)
        {
        }

        [[nodiscard]] constexpr auto empty() const -> bool
        {
            return (m_line == 0);
        }
    };

    struct Message
    {
        struct MessageOptions
        {
            util::time::timepoint time = {};
            std::string_view name      = {};
            std::string_view msg       = {};
            Location loc               = {};
            Level level                = {};
        };

        MessageOptions m_options = {};
        usize m_thread           = { 0U };

        mutable usize m_color_start = { 0U };
        mutable usize m_color_end   = { 0U };

        std::string_view m_payload;

        Message()                                    = default;
        Message(Message const &)                     = default;
        auto operator=(Message const &) -> Message & = default;

        explicit Message(MessageOptions options)
        {
        }

        explicit Message(Location loc, std::string_view name, Level level, std::string_view msg)
        {
        }

        explicit Message(std::string_view name, Level level, std::string_view msg)
        {
        }
    };

    class MessageBuffer : public Message
    {
    private:
        std::string m_buffer{};
        auto update() -> void;

    public:
    };

    class Backtracer
    {
    private:
        mutable std::mutex m_mutex = {};
        std::atomic<bool> enabled  = { false };
    };
}  // namespace util::log::details

namespace util::log::sinks
{

}
