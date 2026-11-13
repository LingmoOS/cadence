#pragma once

#include <algorithm>
#include <chrono>
#include <functional>

#include "cadence/clock.h"
#include "cadence/easing.h"

namespace Cadence {

class Timeline {
public:
    using Duration = std::chrono::nanoseconds;
    static constexpr int Infinite = -1;

    void setDuration(Duration duration) { m_duration = std::max(Duration{1}, duration); }
    void setEasing(Easing easing) { m_easing = easing; }
    void setLoops(int loops) { m_loops = loops == Infinite ? Infinite : std::max(1, loops); }
    void setAlternating(bool alternating) { m_alternating = alternating; }

    void start(TimePoint now)
    {
        m_start = now;
        m_elapsedBeforePause = Duration::zero();
        m_running = true;
        m_paused = false;
        m_progress = 0;
        if (onUpdate)
            onUpdate(m_progress);
    }

    void start(const Clock &clock) { start(clock.now()); }

    void stop() { m_running = m_paused = false; }
    void pause(TimePoint now)
    {
        if (!m_running || m_paused)
            return;
        m_elapsedBeforePause += elapsedSince(m_start, now);
        m_paused = true;
    }
    void pause(const Clock &clock) { pause(clock.now()); }
    void resume(TimePoint now)
    {
        if (!m_running || !m_paused)
            return;
        m_start = now;
        m_paused = false;
    }
    void resume(const Clock &clock) { resume(clock.now()); }

    bool advance(TimePoint now)
    {
        if (!m_running || m_paused)
            return false;
        const Duration elapsed = m_elapsedBeforePause + elapsedSince(m_start, now);
        const double cycles = static_cast<double>(elapsed.count()) / static_cast<double>(m_duration.count());
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

    bool advance(const Clock &clock) { return advance(clock.now()); }

    bool isRunning() const { return m_running; }
    bool isPaused() const { return m_paused; }
    double progress() const { return m_progress; }

    std::function<void(double)> onUpdate;
    std::function<void()> onFinished;

private:
    static Duration elapsedSince(TimePoint start, TimePoint now) { return now > start ? now - start : Duration::zero(); }

    Duration m_duration = std::chrono::milliseconds(250);
    Easing m_easing = Easing::InOutCubic;
    int m_loops = 1;
    bool m_alternating = false;
    bool m_running = false;
    bool m_paused = false;
    TimePoint m_start = Duration::zero();
    Duration m_elapsedBeforePause = Duration::zero();
    double m_progress = 0;
};

} // namespace Cadence
