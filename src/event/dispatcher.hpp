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
    /***************************************************************************************************
     * Used to forward events from an emitter to many subscribers
     *
     * @tparam Derived specific `Dispatcher` implementation
     **************************************************************************************************/
    template <class Derived>
    class Dispatcher : public Provider<Derived>
    {
    private:
        /** Concrete instance type */
        using self_type = Dispatcher<Derived>;

        /** Concrete provider type */
        using provider_type = Provider<Derived>;

        /** Type that derives the instance */
        using derived_type = Derived;

        /** Type of the subscriber */
        using subscriber_type = Subscriber;

        /***********************************************************************************************
         * Constraints the `Derived` type
         **********************************************************************************************/
        constexpr auto check_static() -> void
        {
            static_assert(Dispatchable<Derived>);
        }

    public:
        /***********************************************************************************************
         * Forwards the `dispatch` method to the concrete implementation
         *
         * @param event event to forward
         **********************************************************************************************/
        auto dispatch(Event const &event) -> void
        {
            this->derived()->dispatch(event);
        }
    };
}  // namespace event
