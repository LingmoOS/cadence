#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <type_traits>

namespace Cadence {

template<typename T, typename Enable = void>
struct Interpolator;

template<typename T>
struct Interpolator<T, std::enable_if_t<std::is_arithmetic_v<T>>> {
    static T mix(T from, T to, double progress)
    {
        return static_cast<T>(static_cast<double>(from) + (static_cast<double>(to) - from) * progress);
    }
};

struct Color {
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 255;

    bool operator==(const Color &) const = default;
};

template<>
struct Interpolator<Color> {
    static Color mix(Color from, Color to, double progress)
    {
        const auto channel = [progress](uint8_t a, uint8_t b) {
            return static_cast<uint8_t>(std::lround(a + (static_cast<double>(b) - a) * progress));
        };
        return {channel(from.r, to.r), channel(from.g, to.g), channel(from.b, to.b), channel(from.a, to.a)};
    }
};

} // namespace Cadence
