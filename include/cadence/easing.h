#pragma once

#include <algorithm>
#include <cmath>

namespace Cadence {

enum class Easing {
    Linear,
    InQuad,
    OutQuad,
    InOutQuad,
    OutCubic,
    InOutCubic,
    OutQuint,
    OutSine,
};

inline double ease(Easing curve, double progress)
{
    const double t = std::clamp(progress, 0.0, 1.0);
    switch (curve) {
    case Easing::Linear:
        return t;
    case Easing::InQuad:
        return t * t;
    case Easing::OutQuad:
        return 1 - (1 - t) * (1 - t);
    case Easing::InOutQuad:
        return t < 0.5 ? 2 * t * t : 1 - std::pow(-2 * t + 2, 2) / 2;
    case Easing::OutCubic:
        return 1 - std::pow(1 - t, 3);
    case Easing::InOutCubic:
        return t < 0.5 ? 4 * t * t * t : 1 - std::pow(-2 * t + 2, 3) / 2;
    case Easing::OutQuint:
        return 1 - std::pow(1 - t, 5);
    case Easing::OutSine:
        return std::sin(t * 1.57079632679489661923);
    }
    return t;
}

} // namespace Cadence
