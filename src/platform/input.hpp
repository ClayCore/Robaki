/** @file input.hpp */

#pragma once

// module includes
#include "event/listener.hpp"
#include "event/object.hpp"
#include "util/util.hpp"

// c++ includes
#include <utility>


namespace platform
{
    struct Button : public event::Object
    {
    private:
        usize m_id;
        std::string m_name;

        bool m_down;
        bool m_last, m_last_tick;
        bool m_pressed, m_pressed_tick;

    public:
        Button() = default;
        Button(usize id, std::string name) : m_id(id), m_name(std::move(name))
        {
        }

        constexpr auto update() -> void
        {
            m_pressed = m_down && !m_last;
            m_last    = m_down;
        }

        constexpr auto tick() -> void
        {
            m_pressed_tick = m_down && !m_last_tick;
            m_last_tick    = m_down;
        }
    };

    template <typename Derived>
    class Input
    {
    };
}  // namespace platform
