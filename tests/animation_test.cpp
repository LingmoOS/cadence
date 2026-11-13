#include <gtest/gtest.h>

#include <chrono>

#include "cadence/cadence.h"

using namespace Cadence;
using namespace std::chrono_literals;

TEST(Easing, ClampsInputAndReachesEndpoints)
{
    EXPECT_DOUBLE_EQ(0, ease(Easing::OutCubic, -1));
    EXPECT_DOUBLE_EQ(1, ease(Easing::OutCubic, 1));
    EXPECT_DOUBLE_EQ(1, ease(Easing::OutCubic, 2));
}

TEST(Timeline, AdvancesRepeatsAndAlternates)
{
    Timeline timeline;
    timeline.setDuration(100ns);
    timeline.setEasing(Easing::Linear);
    timeline.setLoops(2);
    timeline.setAlternating(true);
    timeline.start(1000ns);

    EXPECT_TRUE(timeline.advance(1050ns));
    EXPECT_DOUBLE_EQ(0.5, timeline.progress());
    EXPECT_TRUE(timeline.advance(1150ns));
    EXPECT_DOUBLE_EQ(0.5, timeline.progress());
    EXPECT_FALSE(timeline.advance(1200ns));
    EXPECT_DOUBLE_EQ(0, timeline.progress());
}

TEST(Timeline, PauseAndResumePreserveElapsedTime)
{
    Timeline timeline;
    timeline.setDuration(1000ns);
    timeline.setEasing(Easing::Linear);
    ManualClock clock(100ns);
    timeline.start(clock);
    clock.advance(300ns);
    timeline.advance(clock);
    timeline.pause(clock);
    clock.advance(300ns);
    EXPECT_FALSE(timeline.advance(clock));
    timeline.resume(clock);
    clock.advance(100ns);
    timeline.advance(clock);
    EXPECT_DOUBLE_EQ(0.4, timeline.progress());
}

TEST(Tween, RetargetsValuesAndInterpolatesColor)
{
    Tween<double> number;
    number.setDuration(100ns);
    number.setEasing(Easing::Linear);
    number.setValue(2);
    number.animateTo(10, 0ns);
    number.advance(50ns);
    EXPECT_DOUBLE_EQ(6, number.value());

    Tween<Color> color;
    color.setDuration(100ns);
    color.setEasing(Easing::Linear);
    color.animateTo({255, 100, 0, 255}, {0, 0, 0, 255}, 0ns);
    color.advance(50ns);
    EXPECT_EQ((Color{128, 50, 0, 255}), color.value());
}

TEST(Spring, SettlesTowardsItsTarget)
{
    Spring spring;
    spring.snap(0);
    spring.setTarget(1);
    for (int i = 0; i < 240; ++i)
        spring.advance(std::chrono::duration<double>(1.0 / 60.0));
    EXPECT_NEAR(1, spring.value(), 0.001);
    EXPECT_TRUE(spring.isSettled());
}

TEST(Tween, RetargetStartsFromTheCurrentValue)
{
    Tween<double> tween;
    tween.setDuration(100ns);
    tween.setEasing(Easing::Linear);
    tween.animateTo(10, 0ns);
    tween.advance(40ns);
    tween.animateTo(0, 40ns);
    tween.advance(70ns);
    EXPECT_DOUBLE_EQ(6, tween.value());
}

TEST(Clock, ManualClockAdvancesDeterministically)
{
    ManualClock clock(5ns);
    clock.advance(12ns);
    EXPECT_EQ(17ns, clock.now());
}
