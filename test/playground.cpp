/** \file
 * Main compilation unit of the fpm library.
 */

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <type_traits>

#include <fpm.hpp>

using namespace fpm::types;
using Ovf = fpm::Ovf;

using pos_t = fpm::q::Q<int, 16, -2000., 2000. /* mm */>;
FPM_Q_BIND_LITERAL(pos_t, mm);
using speed_t = i32q16<-300., 300. /* mm/s */>;
FPM_Q_BIND_LITERAL(speed_t, mm_p_s);
using accel_t = i32q16<-200., 200. /* mm/s2 */>;
FPM_Q_BIND_LITERAL(accel_t, mm_p_s2);
using mtime_t = i32q20<-2000., 2000. /* s */>;
FPM_Q_BIND_LITERAL(mtime_t, s);


void accel(pos_t &position, speed_t &velocity, accel_t const acceleration, mtime_t const forTime, i32q20<0., .1> const dt) {
    for (mtime_t t = 0_s; t < forTime; t = mtime_t::fromSq<Ovf::noCheck>(t + dt)) {
        auto dv = +acceleration * dt;
        auto ds = velocity * dt;
        velocity = speed_t::fromSq<Ovf::clamp>(velocity + dv);
        position = pos_t::fromSqClamp(position + ds);
    }
}


void playground() {
    pos_t position = -10.2_mm * 2_ic;
    speed_t velocity = 0_mm_p_s;

    fpm::static_assert_basetype<int32_t, decltype(velocity)>();
    fpm::static_assert_scaling<16, decltype(velocity)>();
    fpm::static_assert_range<-300., +300., decltype(velocity)>();
    fpm::static_assert_properties<int32_t, 16, -300., +300., decltype(velocity)>();

    accel(position, velocity, max(-100_mm_p_s2, -120_mm_p_s2), 1_s, min(1e-3_s, 1e-2_s));

    // limit results
    position = position % 100_mm;
    velocity = clamp(velocity, -100_mm_p_s, -10_mm_p_s);  // runtime limits
    // compile-time limits; value is usually clamped at runtime though
    velocity = clamp<-100., -10.>(velocity);
    velocity = clamp<-99.9_mm, -9.9_mm>(velocity);  // also works with literals

    velocity = sqrt(clamp<2_mm, 100_mm>(-velocity));

    std::cout << "pos size:" << sizeof(position) << ", spd size:" << sizeof(velocity) << std::endl;
    std::cout << "pos: " << position.real() << ", vel: " << velocity.real() << std::endl;
}


int main(void) {
    playground();
    return 0;
}


// EOF
