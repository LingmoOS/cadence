#pragma once

#include <cstdint>

namespace Cadence {

using Nanoseconds = uint64_t;

class Clock {
public:
    virtual ~Clock() = default;
    virtual Nanoseconds now() const = 0;
};

class ManualClock final : public Clock {
public:
    explicit ManualClock(Nanoseconds initial = 0) : m_now(initial) {}

    Nanoseconds now() const override { return m_now; }
    void set(Nanoseconds now) { m_now = now; }
    void advance(Nanoseconds delta) { m_now += delta; }

private:
    Nanoseconds m_now;
};

} // namespace Cadence
