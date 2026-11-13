#pragma once

#include <algorithm>
#include <cstdint>
#include <functional>

#include "cadence/clock.h"
#include "cadence/easing.h"

namespace Cadence {

class Timeline {
public:
    using Duration = uint64_t;
    static constexpr int Infinite = -1;

    void setDuration(Duration duration) { m_duration = std::max<Duration>(1, duration); }
    void setEasing(Easing easing) { m_easing = easing; }
    void setLoops(int loops) { m_loops = loops; }
    void setAlternating(bool alternating) { m_alternating = alternating; }

    void start(Nanoseconds now)
    {
        m_start = now;
        m_elapsedBeforePause = 0;
        m_running = true;
        m_paused = false;
        m_progress = 0;
        if (onUpdate)
            onUpdate(m_progress);
    }

    void stop() { m_running = m_paused = false; }
    void pause(Nanoseconds now)
    {
        if (!m_running || m_paused)
            return;
        m_elapsedBeforePause += now - m_start;
        m_paused = true;
    }
    void resume(Nanoseconds now)
    {
        if (!m_running || !m_paused)
            return;
        m_start = now;
        m_paused = false;
    }

    bool advance(Nanoseconds now)
    {
        if (!m_running || m_paused)
            return false;
        const Nanoseconds elapsed = m_elapsedBeforePause + (now - m_start);
        const double cycles = static_cast<double>(elapsed) / static_cast<double>(m_duration);
        if (m_loops != Infinite && cycles >= m_loops) {
            m_progress = m_alternating && m_loops > 0 && m_loops % 2 == 0 ? 0 : 1;
            m_running = false;
            if (onUpdate)
                onUpdate(m_progress);
            if (onFinished)
                onFinished();
            return false;
        }
        const auto cycle = static_cast<uint64_t>(cycles);
        double t = cycles - static_cast<double>(cycle);
        if (m_alternating && cycle % 2)
            t = 1 - t;
        m_progress = ease(m_easing, t);
        if (onUpdate)
            onUpdate(m_progress);
        return true;
    }

    bool isRunning() const { return m_running; }
    bool isPaused() const { return m_paused; }
    double progress() const { return m_progress; }

    std::function<void(double)> onUpdate;
    std::function<void()> onFinished;

private:
    Duration m_duration = 250'000'000;
    Easing m_easing = Easing::InOutCubic;
    int m_loops = 1;
    bool m_alternating = false;
    bool m_running = false;
    bool m_paused = false;
    Nanoseconds m_start = 0;
    Nanoseconds m_elapsedBeforePause = 0;
    double m_progress = 0;
};

} // namespace Cadence
