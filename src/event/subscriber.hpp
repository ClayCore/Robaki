/** @file subscriber.hpp */

#pragma once

// module includes
#include "listener.hpp"
#include "util/util.hpp"

// c++ includes
#include <algorithm>

namespace event
{
    /**
     * Used by all objects that respond to events.
     */
    class Subscriber
    {
    private:
        /**
         * Concrete `Listener` implementation for the `Subscriber`
         *
         * Provides a method to call every function tied to any received
         * events.
         */
        class Listener : public event::Listener<Subscriber>
        {
        public:
            /**
             * Calls all mapped functors associated with the provided event
             *
             * @param event subscribed event
             */
            auto listen(Event const &event) -> void
            {
                auto call      = [](Callback &callback) -> void { callback(); };
                auto callbacks = m_actions[event];

                std::for_each(callbacks.begin(), callbacks.end(), call);
            }
        };

        /** Bound listener */
        Subscriber::Listener m_listener;

    public:
        // ================================================================================================ //
        // Constructors and destructors =================================================================== //
        // ================================================================================================ //

        Subscriber()                              = default;
        ~Subscriber()                             = default;
        Subscriber(const Subscriber &)            = default;
        Subscriber(Subscriber &&)                 = delete;
        Subscriber &operator=(const Subscriber &) = default;
        Subscriber &operator=(Subscriber &&)      = delete;

        // ================================================================================================ //
        // Accessor methods =============================================================================== //
        // ================================================================================================ //

        /**
         * Returns listener bound to the subscriber
         *
         * @return bound listener
         */
        auto get_listener() -> Subscriber::Listener &
        {
            return (m_listener);
        }
    };
}  // namespace event
