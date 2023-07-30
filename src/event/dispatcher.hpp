/** @file dispatcher.hpp */

#pragma once

// module includes
#include "event.hpp"
#include "util/util.hpp"

namespace event
{
    template <class Derived>
    class Dispatcher
    {
    protected:
        std::vector<Event> m_events;

        ~Dispatcher() = default;

    public:
        Dispatcher()
        {
            m_events = {};
        }

        auto derived() -> Derived *
        {
            return (static_cast<Derived *>(this));
        }

        auto derived() const -> Derived const *
        {
            return (static_cast<Derived const *>(this));
        }

        auto get_event(usize index) const -> Event
        {
            this->derived().get_event(index);
        }

        auto set_event(Event const &event, usize index) -> void
        {
            this->derived().set_event(event, index);
        }

        auto add_event(Event const &event) -> void
        {
            this->derived().add_event(event);
        }

        template <class Emitter>
        auto dispatch(Event const &event, Emitter &emitter) -> void
        {
            this->derived().dispatch(event, emitter);
        }
    };
}  // namespace event
