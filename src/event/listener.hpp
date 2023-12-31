/** @file listener.hpp */

#pragma once

// module includes
#include "event.hpp"
#include "provider.hpp"
#include "util/util.hpp"


// c++ includes
#include <unordered_map>

namespace event
{
    /***************************************************************************************************
     * Used to *listen to* events from any dispatchers and call callbacks associated with them
     *
     * @tparam Derived concrete `Listener` implementation
     **************************************************************************************************/
    template <class Derived>
    class Listener: public Provider<Derived>
    {
    private:
        /** Concrete instance type */
        using self_type = Listener<Derived>;

        /** Concrete provider type */
        using provider_type = Provider<Derived>;

        /** Type that derives the instance */
        using derived_type = Derived;

        /***********************************************************************************************
         * Constraints the `Derived` type
         **********************************************************************************************/
        auto constexpr check_static() -> void
        {
            static_assert(Dispatchable<Derived>);
        }

    public:
        /***********************************************************************************************
         * Forwards the `listen` method to the concrete implementation
         *
         * @param event subscribed event
         **********************************************************************************************/
        auto listen(Event const &event) -> void
        {
            this->derived()->listen(event);
        }
    };
}  // namespace event
