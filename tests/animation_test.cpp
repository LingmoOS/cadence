#include <gtest/gtest.h>

#include "cadence/cadence.h"

using namespace Cadence;

TEST(Easing, ClampsInputAndReachesEndpoints)
{
    EXPECT_DOUBLE_EQ(0, ease(Easing::OutCubic, -1));
    EXPECT_DOUBLE_EQ(1, ease(Easing::OutCubic, 1));
    EXPECT_DOUBLE_EQ(1, ease(Easing::OutCubic, 2));
}

TEST(Timeline, AdvancesRepeatsAndAlternates)
{
    Timeline timeline;
    timeline.setDuration(100);
    timeline.setEasing(Easing::Linear);
    timeline.setLoops(2);
    timeline.setAlternating(true);
    timeline.start(1000);

    EXPECT_TRUE(timeline.advance(1050));
    EXPECT_DOUBLE_EQ(0.5, timeline.progress());
    EXPECT_TRUE(timeline.advance(1150));
    EXPECT_DOUBLE_EQ(0.5, timeline.progress());
    EXPECT_FALSE(timeline.advance(1200));
    EXPECT_DOUBLE_EQ(0, timeline.progress());
}

TEST(Timeline, PauseAndResumePreserveElapsedTime)
{
    Timeline timeline;
    timeline.setDuration(1000);
    timeline.setEasing(Easing::Linear);
    timeline.start(100);
    timeline.advance(400);
    timeline.pause(400);
    EXPECT_FALSE(timeline.advance(700));
    timeline.resume(900);
    timeline.advance(1000);
    EXPECT_DOUBLE_EQ(0.4, timeline.progress());
}

TEST(Tween, RetargetsValuesAndInterpolatesColor)
{
    Tween<double> number;
    number.setDuration(100);
    number.setEasing(Easing::Linear);
    number.animateTo(10, 2, 0);
    number.advance(50);
    EXPECT_DOUBLE_EQ(6, number.value());

    Tween<Color> color;
    color.setDuration(100);
    color.setEasing(Easing::Linear);
    color.animateTo({255, 100, 0, 255}, {0, 0, 0, 255}, 0);
    color.advance(50);
    EXPECT_EQ((Color{128, 50, 0, 255}), color.value());
}

TEST(Spring, SettlesTowardsItsTarget)
{
    Spring spring;
    spring.snap(0);
    spring.setTarget(1);
    for (int i = 0; i < 240; ++i)
        spring.advance(1.0 / 60.0);
    EXPECT_NEAR(1, spring.value(), 0.001);
    EXPECT_TRUE(spring.isSettled());
}
