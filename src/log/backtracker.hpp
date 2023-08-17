/** @file backtracker.hpp */

#pragma once

// module includes
#include "util/util.hpp"

// c++ includes
#include <atomic>
#include <mutex>


namespace logging
{
    class Backtracer
    {
    private:
        std::mutex mutable m_mutex  = {};
        std::atomic<bool> m_enabled = { false };
    };
}  // namespace logging
