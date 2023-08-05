/** @file state.hpp */

#pragma once

// module includes
#include "platform/platform.hpp"
#include "util/util.hpp"


namespace state
{
    struct State : public Singleton<State>
    {
        platform::Platform platform;
    };
}  // namespace state
