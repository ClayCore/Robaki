/** @file dispatcher.hpp */

#pragma once

// module includes
#include "event.hpp"
#include "provider.hpp"
#include "subscriber.hpp"
#include "util/util.hpp"


// c++ includes
#include <unordered_map>

namespace event
{
    template <typename Derived>
    class Dispatcher : public Provider<Derived>
    {
    private:
        using self_type     = Dispatcher<Derived>;
        using provider_type = Provider<Derived>;
        using derived_type  = Derived;
        using object_type   = Subscriber;

    public:
        auto dispatch(Event const &event) -> void
        {
            this->derived()->dispatch(event);
        }
    };
}  // namespace event
