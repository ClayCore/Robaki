/** @file emitter.hpp */

#pragma once

// module includes
#include "event.hpp"
#include "handler.hpp"
#include "util/util.hpp"

namespace event
{
    template <typename Derived>
    class Emitter : public Handler
    {
    protected:
        using self_type    = Emitter<Derived>;
        using derived_type = Derived;

        std::vector<Handler *> m_dispatchers;

        auto derived() const -> Derived const *
        {
            return (static_cast<Derived const *>(this));
        }

        auto derived() -> Derived *
        {
            return (static_cast<Derived *>(this));
        }

    public:
        [[nodiscard]] auto get_event(usize index) const -> Event
        {
            return (this->derived()->get_event(index));
        }

        auto set_event(Event const &event, usize index) -> void
        {
            this->derived()->set_event(event, index);
        }

        auto add_event(Event const &event) -> void
        {
            this->derived()->add_event(event);
        }

        auto emit(Event const &event) -> void final
        {
            this->derived()->emit(event);
        }
    };
}  // namespace event
