/** @file listener.hpp */

#pragma once

// module includes
#include "event.hpp"
#include "util/util.hpp"

// c++ includes
#include <unordered_map>

namespace event
{
    template <class Derived>
    class Listener
    {
    protected:
        using Callback = void (*)();
        std::unordered_map<Event, Callback> m_actions;

        Listener()
        {
            m_actions = {};
        }

        auto derived() -> Derived *
        {
            return (static_cast<Derived *>(this));
        }

        auto derived() const -> Derived const *
        {
            return (static_cast<Derived const *>(this));
        }

        auto get_callback(Event const &event) -> Callback
        {
            return (this->derived().get_callback(event));
        }

        auto set_callback(Event const &event, Callback const &cb) -> void
        {
            this->derived().set_callback(event, cb);
        }

        auto insert_event(Event const &event, Callback const &cb) -> void
        {
            this->derived().insert_event(event, cb);
        }
    };
}  // namespace event
