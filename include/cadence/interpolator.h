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

struct Point {
    double x = 0;
    double y = 0;

    bool operator==(const Point &) const = default;
};

struct Size {
    double width = 0;
    double height = 0;

    bool operator==(const Size &) const = default;
};

struct Rect {
    double x = 0;
    double y = 0;
    double width = 0;
    double height = 0;

    bool operator==(const Rect &) const = default;
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

template<>
struct Interpolator<Point> {
    static Point mix(Point from, Point to, double progress)
    {
        return {Interpolator<double>::mix(from.x, to.x, progress),
                Interpolator<double>::mix(from.y, to.y, progress)};
    }
};

template<>
struct Interpolator<Size> {
    static Size mix(Size from, Size to, double progress)
    {
        return {Interpolator<double>::mix(from.width, to.width, progress),
                Interpolator<double>::mix(from.height, to.height, progress)};
    }
};

template<>
struct Interpolator<Rect> {
    static Rect mix(Rect from, Rect to, double progress)
    {
        return {Interpolator<double>::mix(from.x, to.x, progress),
                Interpolator<double>::mix(from.y, to.y, progress),
                Interpolator<double>::mix(from.width, to.width, progress),
                Interpolator<double>::mix(from.height, to.height, progress)};
    }
};

} // namespace Cadence
