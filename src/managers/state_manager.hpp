/** @file state_manager.hpp */

#pragma once

// module includes
#include "event_manager.hpp"
#include "managers/event_manager.hpp"
#include "state/state.hpp"
#include "util/util.hpp"


namespace managers
{
    class StateManager : public Singleton<StateManager>
    {
    private:
        state::State m_state;

    public:
    };
}  // namespace managers
