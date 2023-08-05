/** @file platform.cpp */

// module includes
#include "platform.hpp"

namespace platform
{
    auto Platform::update() -> void
    {
        for (auto &[_, input] : m_inputs) {
            input->update();
        }
    }

    auto Platform::tick() -> void
    {
        for (auto &[_, input] : m_inputs) {
            input->tick();
        }
    }
}  // namespace platform
