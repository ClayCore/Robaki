/** @file listener.hpp */

#pragma once

// module includes
#include "event.hpp"
#include "event/event.hpp"
#include "util/util.hpp"

// c++ includes
#include <unordered_map>

namespace event
{
    template <class Derived>
    class Listener
    {
    protected:
        using self_type    = Listener<Derived>;
        using derived_type = Derived;

        using Callback = void (*)();
        std::unordered_map<Event, Callback, EventHasher, EventEqualizer> m_actions;

        ~Listener() = default;

        auto derived() -> Derived *
        {
            return (static_cast<Derived *>(this));
        }

        auto derived() const -> Derived const *
        {
            return (static_cast<Derived const *>(this));
        }

    public:
        Listener()
        {
            m_actions = {};
        }

        auto set_callback(Event const &event, Callback &callback) -> void
        {
            this->derived()->set_callback(event, callback);
        }

        auto get_callback(Event const &event) -> Callback &
        {
            return (this->derived()->get_callback(event));
        }

        auto listen(Event const &event) -> void
        {
            this->derived()->listen(event);
        }
    };
}  // namespace event
