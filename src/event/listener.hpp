/** @file listener.hpp */

#pragma once

// module includes
#include "event.hpp"
#include "event/event.hpp"
#include "provider.hpp"
#include "util/util.hpp"


// c++ includes
#include <unordered_map>

namespace event
{
    template <typename Derived>
    class Listener : public Provider<Derived>
    {
    private:
        using self_type     = Listener<Derived>;
        using provider_type = Provider<Derived>;
        using derived_type  = Derived;

    public:
        auto listen(Event const &event) -> void
        {
            this->derived()->listen(event);
        }
    };
}  // namespace event
