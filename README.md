# Cadence

Cadence is a standalone, header-only C++20 library for deterministic, frame-driven animation. It has no window-system or toolkit dependency; applications supply frame timestamps and connect updates to their own rendering loop.

## First-version scope

- `Timeline`: duration, easing, loops, alternating playback, pause and resume.
- `Tween<T>`: timestamp-driven interpolation for arithmetic types and `Color`.
- `Spring`: closed-form critically damped scalar motion advanced by frame delta.
- `ManualClock`: a small controllable clock for tests and adapters.

The API is an initial foundation. It does not yet provide a scheduler, GUI adapters, keyframes, cancellation tokens, or automatic frame subscription.

## Build and test

```sh
cmake -S . -B build -DCADENCE_BUILD_TESTS=ON
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

## Use

```cpp
#include <cadence/cadence.h>

Cadence::Tween<double> fade;
fade.setDuration(180'000'000); // nanoseconds
fade.setEasing(Cadence::Easing::OutCubic);
fade.onValueChanged = [](double opacity) { /* update a view */ };
fade.animateTo(1.0, 0.0, frameTimeNs);

// Call once per rendered frame. The host owns frame scheduling.
fade.advance(nextFrameTimeNs);
```

The caller should pass monotonic timestamps and continue advancing a tween until `isRunning()` becomes false. The Spring `advance()` argument is elapsed seconds, normally the monotonic frame delta.

## Install

```sh
cmake --install build --prefix /usr/local
```

Downstream CMake projects can use `find_package(Cadence CONFIG REQUIRED)` and link `Cadence::Cadence`.

## License

BSD-3-Clause; see [LICENSE](LICENSE).
