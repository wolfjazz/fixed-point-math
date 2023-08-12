/** \file
 * Main compilation unit of the fpm library.
 */

#include <iostream>

#include "fpm.hpp"
using namespace fpm::type;


using pos_t = fpm::q<int, 16, -2000., 2000.>;  // mm
using speed_t = i32q16<-300., 300.>;  // mm_p_s
using accel_t = i32q16<-200., 200.>;  // mm_p_s2
using mtime_t = i32q20<-2000., 2000.>;  // s
template< char ...chars > consteval auto operator ""_mm() { return fpm::qFromLiteral<pos_t, chars...>(); }
template< char ...chars > consteval auto operator ""_mm_p_s() { return fpm::qFromLiteral<speed_t, chars...>(); }
template< char ...chars > consteval auto operator ""_mm_p_s2() { return fpm::qFromLiteral<accel_t, chars...>(); }
template< char ...chars > consteval auto operator ""_s() { return fpm::qFromLiteral<mtime_t, chars...>(); }


void accel(pos_t &position, speed_t &velocity, accel_t const acceleration, mtime_t const forTime, auto const dt) {
    for (mtime_t t = 0_s; t < forTime; t = mtime_t::fromSqOvf(t + dt)) {
        auto dv = acceleration * dt;
        auto ds = velocity * dt;
        velocity = speed_t::fromSqSat(velocity + dv);
        position = pos_t::fromSqSat(position + ds);
    }
}


void playground() {
    pos_t position = -10.2_mm;
    speed_t velocity = 0_mm_p_s;

    accel(position, velocity, -100_mm_p_s2, 1_s, 1e-3_s);

    // limit position via modulo
    position = position % 50_mm;

    std::cout << "pos size:" << sizeof(position) << ", spd size:" << sizeof(velocity) << std::endl;
    std::cout << "pos: " << position.toReal() << ", vel: " << velocity.toReal() << std::endl;
}

int main(void) {
    playground();
    return 0;
}


// EOF
