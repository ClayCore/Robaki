/** @file emitter.hpp */

#pragma once

// module includes
#include "event.hpp"
#include "util/util.hpp"

namespace event
{
    template <typename Derived, typename Dispatcher>
    class Emitter
    {
    protected:
        using self_type       = Emitter<Derived, Dispatcher>;
        using derived_type    = Derived;
        using dispatcher_type = Dispatcher;

        std::vector<Event> m_events;

        ~Emitter() = default;

        auto derived() const -> Derived const *
        {
            return (static_cast<Derived const *>(this));
        }

        auto derived() -> Derived *
        {
            return (static_cast<Derived *>(this));
        }

    public:
        Emitter()
        {
            m_events = {};
        }

        auto get_event(usize index) const -> Event
        {
            this->derived()->get_event(index);
        }

        auto set_event(Event const &event, usize index) -> void
        {
            this->derived()->set_event(event, index);
        }

        auto add_event(Event const &event) -> void
        {
            this->derived()->add_event(event);
        }

        auto emit(Event const &event, Dispatcher &dispatcher) -> void
        {
            this->derived()->emit(event, dispatcher);
        }
    };
}  // namespace event
