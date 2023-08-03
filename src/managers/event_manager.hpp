/** @file event_manager.hpp */

#pragma once

// module includes
#include "event/dispatcher.hpp"
#include "event/listener.hpp"
#include "util/util.hpp"


namespace managers
{
    class EventManager : public Singleton<EventManager>
    {
    private:
        class Dispatcher : public event::Dispatcher<EventManager::Dispatcher>
        {
        public:
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
