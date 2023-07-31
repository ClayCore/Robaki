/** @file event_manager.hpp */

#pragma once

// module includes
#include "event/dispatcher.hpp"
#include "event/listener.hpp"
#include "event/object.hpp"
#include "util/util.hpp"


namespace managers
{
    class EventManager : public Singleton<EventManager>
    {
    private:
        class Dispatcher : public event::Dispatcher<EventManager>
        {
        public:
            auto dispatch(event::Event const &event)
            {
                for (auto &m_event : m_events) {
                    if (m_event.first != event) {
                        continue;
                    }

                    auto vec = m_event.second;

                    for (auto &j : vec) {
                        j->get_listener().listen(event);
                    }
                }
            }
        };

        Dispatcher m_dispatcher;

    public:
        auto get_dispatcher() -> Dispatcher
        {
            return (m_dispatcher);
        }
    };
}  // namespace managers
