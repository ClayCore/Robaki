/** @file uuid.hpp
 * UUID generation utilities
 */

#pragma once

// c++ includes
#include <random>
#include <sstream>
#include <string>
#include <string_view>

// module includes
#include "types.hpp"

namespace util::uuid
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 15);
    static std::uniform_int_distribution<> dis2(8, 11);

    inline auto generate_uuid() -> std::string
    {
        std::stringstream strstream{};
        u32 iter{};
        strstream << std::hex;

        for (iter = 0; iter < 8; iter++) {
            strstream << dis(gen);
        }
        strstream << "-";
        for (iter = 0; iter < 4; iter++) {
            strstream << dis(gen);
        }
        strstream << "-4";
        for (iter = 0; iter < 3; iter++) {
            strstream << dis(gen);
        }
        strstream << "-";
        strstream << dis2(gen);
        for (iter = 0; iter < 3; iter++) {
            strstream << dis(gen);
        }
        strstream << "-";
        for (iter = 0; iter < 12; iter++) {
            strstream << dis(gen);
        };

        return (strstream.str());
    }
}  // namespace util::uuid
