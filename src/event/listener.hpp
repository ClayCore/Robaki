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
    /**
     * Used to *listen to* events from any dispatchers and call callbacks associated with them
     *
     * @tparam Derived concrete `Listener` implementation
     */
    template <typename Derived>
    class Listener : public Provider<Derived>
    {
    private:
        /** Concrete instance type */
        using self_type = Listener<Derived>;

        /** Concrete provider type */
        using provider_type = Provider<Derived>;

        /** Type that derives the instance */
        using derived_type = Derived;


    public:
        /**
         * Forwards the `listen` method to the concrete implementation
         *
         * @param event subscribed event
         */
        auto listen(Event const &event) -> void
        {
            this->derived()->listen(event);
        }
    };
}  // namespace event
