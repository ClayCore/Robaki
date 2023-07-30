/** @file event.hpp */

#pragma once

// module includes
#include "util/util.hpp"

// fmt includes
#include "fmt/format.h"

namespace event
{
    enum class EventType
    {
        None = 0,
        WindowClose,
        WindowResize,
        WindowFocus,
        WindowLostFocus,
        WindowMoved,
        LoadResource,
        FreeResource,
        EngineTick,
        EngineUpdate,
        EngineRender,
        KeyPressed,
        KeyReleased,
        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        MouseScrolled,
        Custom
    };

    enum class EventCategory
    {
        None = 0,
        Window,
        Resource,
        Engine,
        Input,
        Custom
    };

    class Event
    {
    private:
        EventType m_type;
        EventCategory m_category;
        std::string m_name;

    public:
        Event();
        Event(EventType const &);
        virtual ~Event()
        {
        }

        constexpr auto get_name() const -> char const *
        {
            if (std::empty(m_name)) {
                return (this->get_name_from_type(m_type));
            } else {
                return (m_name.c_str());
            }
        }

        constexpr auto get_type() const -> EventType
        {
            return (m_type);
        }

        constexpr auto get_category() const -> EventCategory
        {
            return (m_category);
        }

        virtual auto get_name_from_type(EventType const &) const -> char const *;
        virtual auto get_category_from_type(EventType const &) const -> EventCategory;
        virtual auto set_type(EventType const &) -> void;
        virtual auto set_name(std::string const &) -> void;
        virtual auto category_to_string(EventCategory const &) const -> char const *;

        constexpr auto operator==(Event const &rhs) const -> bool
        {
            return (this->get_name() == rhs.get_name());
        }

        constexpr auto operator!=(Event const &rhs) const -> bool
        {
            return (!(*this == rhs));
        }

        constexpr auto operator<=>(Event const &rhs) const -> std::strong_ordering
        {
            return (this->get_type() <=> rhs.get_type());
        }

        auto to_string() const -> std::string;
        friend inline auto operator<<(std::ostream &os, Event const &event) -> std::ostream &
        {
            return (os << event.to_string());
        }
    };
}  // namespace event

template <>
struct fmt::formatter<event::Event> : formatter<std::string>
{
    auto format(event::Event const &event, format_context &ctx) const -> format_context::iterator
    {
        return formatter<std::string>::format(event.to_string(), ctx);
    }
};
