/** \file
 * Main compilation unit of the fpm library.
 */

#include <iostream>

#include "fpm.hpp"
using namespace fpm::type;

using pos_t = i32q16<-2000., 2000.>;  // mm
using speed_t = i32q16<-100., 100.>;  // mm_p_s
using accel_t = i32q16<-200., 200.>;  // mm_p_s2
using mtime_t = i32q10<-3600., 3600.>;  // s
template< char ...chars > consteval auto operator""_mm() { return fpm::qFromLiteral<pos_t, chars...>(); }
template< char ...chars > consteval auto operator""_mm_p_s() { return fpm::qFromLiteral<speed_t, chars...>(); }
template< char ...chars > consteval auto operator""_mm_p_s2() { return fpm::qFromLiteral<accel_t, chars...>(); }
template< char ...chars > consteval auto operator""_s() { return fpm::qFromLiteral<mtime_t, chars...>(); }


void playground() {
    // pos_t position = -10.0_mm;  // TODO: implicit conversion sq->q possible if no overflow check is needed
    // speed_t velocity = 60.6_mm_p_s;
    // accel_t acceleration = 100._mm_p_s2;
    // auto dt = 1e-2_s;
    // mtime_t time = 0_s;

    // for (mtime_t t = time; t < 1_s; t = mtime_t::fromSqOvf(t + dt)) {
    //     auto dv = acceleration * dt;
    //     auto ds = velocity * dt;
    //     velocity = speed_t::fromSqSat(velocity + dv);
    //     position = pos_t::fromSqSat(position + ds);
    // }

    std::cout << "Hello FpmQ :)" << std::endl;
}


int main(void) {
    playground();
    return 0;
}

// EOF
