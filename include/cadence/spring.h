#pragma once

#include <algorithm>
#include <chrono>
#include <cmath>
#include <functional>

namespace Cadence {

// Closed-form critically damped spring; advance with elapsed frame time.
class Spring {
public:
    explicit Spring(double angularFrequency = 18) : m_frequency(std::max(0.01, angularFrequency)) {}

    void setFrequency(double angularFrequency) { m_frequency = std::max(0.01, angularFrequency); }
    void setTarget(double target) { m_target = target; }
    void setInitialVelocity(double velocity) { m_velocity = velocity; }
    void snap(double value)
    {
        m_value = m_target = value;
        m_velocity = 0;
        if (onValueChanged)
            onValueChanged(m_value);
    }

    template<typename Rep, typename Period>
    double advance(std::chrono::duration<Rep, Period> elapsed)
    {
        const double dt = std::max(0.0, std::chrono::duration<double>(elapsed).count());
        const double offset = m_value - m_target;
        const double c = m_velocity + m_frequency * offset;
        const double decay = std::exp(-m_frequency * dt);
        m_value = m_target + (offset + c * dt) * decay;
        m_velocity = (m_velocity - m_frequency * c * dt) * decay;
        if (onValueChanged)
            onValueChanged(m_value);
        return m_value;
    }

    double value() const { return m_value; }
    double target() const { return m_target; }
    double velocity() const { return m_velocity; }
    bool isSettled(double epsilon = 0.001) const
    {
        return std::abs(m_value - m_target) <= epsilon && std::abs(m_velocity) <= epsilon;
    }

    std::function<void(double)> onValueChanged;

private:
    double m_frequency;
    double m_value = 0;
    double m_target = 0;
    double m_velocity = 0;
};

} // namespace Cadence
