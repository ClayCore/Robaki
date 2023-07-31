/** @file dispatcher.hpp */

#pragma once

// module includes
#include "event.hpp"
#include "object.hpp"
#include "util/util.hpp"


// c++ includes
#include <unordered_map>

namespace event
{
    template <typename Derived>
    class Dispatcher
    {
    protected:
        using self_type    = Dispatcher<Derived>;
        using derived_type = Derived;
        using object_type  = Object;

        std::unordered_map<Event, std::vector<Object *>, EventHasher, EventEqualizer> m_events;

        ~Dispatcher() = default;

        auto derived() -> Derived *
        {
            return (static_cast<Derived *>(this));
        }

        auto derived() const -> Derived const *
        {
            return (static_cast<Derived const *>(this));
        }

    public:
        Dispatcher()
        {
            m_events = {};
        }

        auto add_object(Event const &key, Object *obj) -> void
        {
            this->derived()->add_object(key, obj);
        }

        auto remove_object(Event const &key, Object *obj) -> void
        {
            this->derived()->remove_object(key, obj);
        }

        auto add_event(Event const &event, std::vector<Object *> &vec) -> void
        {
            this->derived()->add_event(event, vec);
        }

        auto dispatch(Event const &event) -> void
        {
            this->derived()->dispatch(event);
        }
    };
}  // namespace event
