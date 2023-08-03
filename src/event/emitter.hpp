/** @file emitter.hpp */

#pragma once

// module includes
#include "event.hpp"
#include "provider.hpp"
#include "util/util.hpp"

namespace event
{
    template <typename Derived>
    class Emitter : public Provider<Derived>
    {
    private:
        using self_type     = Emitter<Derived>;
        using provider_type = Provider<Derived>;
        using derived_type  = Derived;

    public:
        auto emit(Event const &event) -> void
        {
            this->derived()->emit(event);
        }
    };
}  // namespace event
