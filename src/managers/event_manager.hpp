/** @file event_manager.hpp */

#pragma once

// module includes
#include "event/dispatcher.hpp"
#include "event/listener.hpp"
#include "util/util.hpp"


namespace managers
{
    /**
     * Used by the engine to forward events from and to the `StateManager`
     */
    class EventManager : public Singleton<EventManager>
    {
    private:
        /**
         * Concrete `Dispatcher` implementation for the `EventManager`
         *
         * Provides a method to forward any received events and sends them
         * to all associated subscribers.
         */
        class Dispatcher : public event::Dispatcher<EventManager::Dispatcher>
        {
        public:
            /**
             * Forwards event to all subscribers and invoke their listeners
             *
             * @param event forwarded event
             */
            auto dispatch(event::Event const &event)
            {
                for (auto const &subscriber : m_subscribers) {
                    subscriber->get_listener().listen(event);
                }
            }
        };

        EventManager::Dispatcher m_dispatcher;

    public:
        auto get_dispatcher() -> EventManager::Dispatcher
        {
            return (m_dispatcher);
        }
    };
}  // namespace managers
