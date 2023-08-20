/** @file state_manager.hpp */

#pragma once

// module includes
#include "event_manager.hpp"
#include "managers/event_manager.hpp"
#include "state/state.hpp"
#include "util/util.hpp"


namespace managers
{
    class StateManager: public Singleton<StateManager>
    {
    public:
        state::State m_state;

        [[nodiscard]] auto get_window() const -> Scope<platform::Window> const &
        {
            return (m_state.platform.m_window);
        }
    };
}  // namespace managers
