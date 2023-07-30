/** @file state_manager.hpp */

#pragma once

// module includes
#include "event/dispatcher.hpp"
#include "event/object.hpp"
#include "util/util.hpp"


namespace state
{
    class StateManager : public Singleton<StateManager>
    {
    private:
        class Dispatcher : public event::Dispatcher<StateManager, event::Object>
        {
        public:
            auto dispatch(event::Event const &event) -> void
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

    protected:
        StateManager()
        {
        }
    };
}  // namespace state
