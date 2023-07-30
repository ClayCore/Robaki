/** @file input.hpp */

#pragma once

// module includes
#include "event/listener.hpp"
#include "util/util.hpp"


namespace platform
{
    struct Button
    {
    private:
        class Listener : public event::Listener<Button>
        {
            auto insert_event(event::Event const &event, Callback const &cb) -> void
            {
                m_actions.insert({ event, cb });
            }
        };

    public:
        Listener m_listener;

        usize m_id;
        std::string m_name;

        bool m_down;
        bool m_last, m_last_tick;
        bool m_pressed, m_pressed_tick;

        Button() = default;
        Button(usize id, std::string name) : m_id(id), m_name(name)
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
}  // namespace platform
