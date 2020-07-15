#pragma once

#include "utils/MurmurHash3.hpp"

namespace hobf
{
    namespace utils
    {
        uint32_t getPower(uint32_t value)
        {
            if (!value)
            {
                throw std::runtime_error("value should be positive");
            }

            if ((value & (value - 1)) != 0)
            {
                throw std::runtime_error("should be power of 2");
            }

            uint32_t x = 0;
            while ((value & (1 << x)) == 0) {
                x += 1;
            }
            return x;
        }

        inline std::pair<uint64_t, uint64_t> hash(std::string_view s, uint32_t seed) noexcept
        {
            std::array<uint64_t, 2> result;
            MurmurHash3_x64_128(s.data(), static_cast<int>(s.size()), seed, result.data());
            return std::make_pair(result[0], result[1]);
        }
    } // namespace utils
} // namespace hobf