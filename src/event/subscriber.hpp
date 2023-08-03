/** @file object.hpp */

#pragma once

// module includes
#include "listener.hpp"
#include "util/util.hpp"

namespace event
{
    class Subscriber
    {
    private:
        class Listener : public event::Listener<Subscriber>
        {
        public:
            auto listen(Event const &event) -> void
            {
                auto call      = [](Callback &callback) -> void { callback(); };
                auto callbacks = m_actions[event];

                std::for_each(callbacks.begin(), callbacks.end(), call);
            }
        };

        Subscriber::Listener m_listener;

    public:
        Subscriber() = default;

        auto get_listener() -> Subscriber::Listener &
        {
            return (m_listener);
        }
    };
}  // namespace event
