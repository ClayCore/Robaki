/** @file object.hpp */

#pragma once

// module includes
#include "listener.hpp"
#include "util/util.hpp"

namespace event
{
    class Object
    {
    private:
        class Listener : public event::Listener<Object>
        {
        public:
            auto listen(Event const &event) -> void
            {
                auto callback = m_actions[event];

                callback();
            }
        };

        Object::Listener m_listener;

    public:
        Object() = default;

        auto get_listener() -> Object::Listener &
        {
            return (m_listener);
        }
    };
}  // namespace event
