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
        class Dispatcher : public event::Dispatcher<EventManager, event::Object>
        {
        public:
            auto dispatch(event::Event const &event)
            {
                for (auto it = m_events.begin(); it != m_events.end(); ++it) {
                    if (it->first != event) {
                        continue;
                    }

                    // auto i = std::distance(m_events.begin(), it);
                    auto vec_obj = it->second;

                    for (auto &j : vec_obj) {
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
