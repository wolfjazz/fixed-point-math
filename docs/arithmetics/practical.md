# Practical Example

```cpp
#include <iostream>

#include <fpm.hpp>

using namespace fpm::types;
using Ovf = fpm::Ovf;


using pos_t = i32q16<-2000., 2000. /* mm */>;
FPM_Q_BIND_LITERAL(pos_t, mm);

using speed_t = i32q16<-300., 300. /* mm/s */>;
FPM_Q_BIND_LITERAL(speed_t, mm_p_s);

using accel_t = i32q16<-200., 200. /* mm/s2 */>;
FPM_Q_BIND_LITERAL(accel_t, mm_p_s2);

using mtime_t = u32q20<0., 2000. /* s */, Ovf::allowed>;
FPM_Q_BIND_LITERAL(mtime_t, s);
using ts_t = mtime_t::clamp_t<0., .01>;  // u32q20


void accel(pos_t &s, speed_t &v, accel_t const a, mtime_t const time, ts_t const dt) {
    for (mtime_t t = 0_s; t < time; t = t + dt) {
        auto dv = a * dt;  // i32sq20<-2., 2.>
        auto ds = v * dt;  // i32sq20<-3., 3.>

        // note: computation results are in Sq representation;
        // explicit conversion back to Q representation is necessary
        v = speed_t::fromSq<Ovf::clamp>(v + dv);
        s = pos_t::fromSq<Ovf::clamp>(s + ds);
    }
}


int main() {
    pos_t position = -10.2_mm;
    speed_t velocity = 0_mm_p_s;

    // acceleration of 100 mm/s^2 for 1 second
    accel(position, velocity, 100_mm_p_s2, 1_s, 1e-3_s);

    // Position: 39.7874, Velocity: 100.045
    std::cout << "Position: " << position.real() << "\n"
              << "Velocity: " << velocity.real() << std::endl;

    // copy position and velocity
    pos_t position2 = position;
    speed_t velocity2 = velocity;

    // clamp velocity2 to 80 mm/s
    velocity2 = clampUpper<80_mm_p_s>(velocity2);

    // deceleration of 50 mm/s^2 for 3 seconds, fewer steps
    accel(position2, velocity2, -50_mm_p_s2, 3_s, 1e-2_s);

    // Position2: 54.8479, Velocity2: -70.4908
    std::cout << "Position2: " << position2.real() << "\n"
              << "Velocity2: " << velocity2.real() << std::endl;

    // take minimum
    auto minPos = min(position, position2);
    auto minVel = min(velocity, velocity2);

    // minPos: 39.7874, minVel: -70.4908
    std::cout << "minPos: " << minPos.real() << "\n"
              << "minVel: " << minVel.real() << std::endl;

    return 0;
}
```
