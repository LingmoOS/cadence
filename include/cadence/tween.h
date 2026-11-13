#pragma once

#include <functional>

#include "cadence/easing.h"
#include "cadence/interpolator.h"
#include "cadence/timeline.h"

namespace Cadence {

template<typename T, typename ValueInterpolator = Interpolator<T>>
class Tween {
public:
    using Duration = Timeline::Duration;

    void setDuration(Duration duration) { m_timeline.setDuration(duration); }
    void setEasing(Easing easing) { m_timeline.setEasing(easing); }
    void setLoops(int loops) { m_timeline.setLoops(loops); }
    void setAlternating(bool alternating) { m_timeline.setAlternating(alternating); }

    void animateTo(T target, T current, Nanoseconds now)
    {
        m_from = current;
        m_target = target;
        m_timeline.onUpdate = [this](double progress) {
            m_value = ValueInterpolator::mix(m_from, m_target, progress);
            if (onValueChanged)
                onValueChanged(m_value);
        };
        m_timeline.onFinished = [this] {
            m_value = m_target;
            if (onFinished)
                onFinished();
        };
        m_timeline.start(now);
    }

    bool advance(Nanoseconds now) { return m_timeline.advance(now); }
    void stop() { m_timeline.stop(); }
    bool isRunning() const { return m_timeline.isRunning(); }
    const T &value() const { return m_value; }
    const T &target() const { return m_target; }

    std::function<void(const T &)> onValueChanged;
    std::function<void()> onFinished;

private:
    T m_from {};
    T m_target {};
    T m_value {};
    Timeline m_timeline;
};

} // namespace Cadence
