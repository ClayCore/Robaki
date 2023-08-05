/** @file emitter.hpp */

#pragma once

// module includes
#include "event.hpp"
#include "provider.hpp"
#include "util/util.hpp"

namespace event
{
    /**
     * Used to *emit* events to many dispatchers.
     *
     * @tparam Derived concrete `Emitter` implementation
     */
    template <typename Derived>
    class Emitter : public Provider<Derived>
    {
    private:
        /** Concrete instance type */
        using self_type = Emitter<Derived>;

        /** Concrete provider type */
        using provider_type = Provider<Derived>;

        /** Type that derives the instance */
        using derived_type = Derived;

        /**
         * Constraints the `Derived` type
         */
        constexpr auto check_static() -> void
        {
            static_assert(util::traits::Emittable<Derived>);
        }

    public:
        /**
         * Forwards the `emit` method to the concrete implementation
         *
         * @param event event to fire
         */
        auto emit(Event const &event) -> void
        {
            this->derived()->emit(event);
        }
    };
}  // namespace event
