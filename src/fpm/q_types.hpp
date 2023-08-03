/** \file
 * Declaration of the predefined q types and literals.
 */

#ifndef FPM_FPM_Q_TYPES_HPP_
#define FPM_FPM_Q_TYPES_HPP_

#include "q.hpp"

namespace fpm::inline type {

/* q types */

template< double lower = details::lowestRealVMin<  int8_t,  -4>(), double upper = details::highestRealVMax<  int8_t,  -4>(), Overflow ovfBx = Ovf::forbidden > using   i8qm4 = q<  int8_t,  -4, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<  int8_t,  -3>(), double upper = details::highestRealVMax<  int8_t,  -3>(), Overflow ovfBx = Ovf::forbidden > using   i8qm3 = q<  int8_t,  -3, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<  int8_t,  -2>(), double upper = details::highestRealVMax<  int8_t,  -2>(), Overflow ovfBx = Ovf::forbidden > using   i8qm2 = q<  int8_t,  -2, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<  int8_t,  -1>(), double upper = details::highestRealVMax<  int8_t,  -1>(), Overflow ovfBx = Ovf::forbidden > using   i8qm1 = q<  int8_t,  -1, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<  int8_t,   0>(), double upper = details::highestRealVMax<  int8_t,   0>(), Overflow ovfBx = Ovf::forbidden > using    i8q0 = q<  int8_t,   0, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<  int8_t,   1>(), double upper = details::highestRealVMax<  int8_t,   1>(), Overflow ovfBx = Ovf::forbidden > using    i8q1 = q<  int8_t,   1, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<  int8_t,   2>(), double upper = details::highestRealVMax<  int8_t,   2>(), Overflow ovfBx = Ovf::forbidden > using    i8q2 = q<  int8_t,   2, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<  int8_t,   3>(), double upper = details::highestRealVMax<  int8_t,   3>(), Overflow ovfBx = Ovf::forbidden > using    i8q3 = q<  int8_t,   3, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<  int8_t,   4>(), double upper = details::highestRealVMax<  int8_t,   4>(), Overflow ovfBx = Ovf::forbidden > using    i8q4 = q<  int8_t,   4, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<  int8_t,   5>(), double upper = details::highestRealVMax<  int8_t,   5>(), Overflow ovfBx = Ovf::forbidden > using    i8q5 = q<  int8_t,   5, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<  int8_t,   6>(), double upper = details::highestRealVMax<  int8_t,   6>(), Overflow ovfBx = Ovf::forbidden > using    i8q6 = q<  int8_t,   6, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<  int8_t,   7>(), double upper = details::highestRealVMax<  int8_t,   7>(), Overflow ovfBx = Ovf::forbidden > using    i8q7 = q<  int8_t,   7, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< uint8_t,  -4>(), double upper = details::highestRealVMax< uint8_t,  -4>(), Overflow ovfBx = Ovf::forbidden > using   u8qm4 = q< uint8_t,  -4, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< uint8_t,  -3>(), double upper = details::highestRealVMax< uint8_t,  -3>(), Overflow ovfBx = Ovf::forbidden > using   u8qm3 = q< uint8_t,  -3, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< uint8_t,  -2>(), double upper = details::highestRealVMax< uint8_t,  -2>(), Overflow ovfBx = Ovf::forbidden > using   u8qm2 = q< uint8_t,  -2, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< uint8_t,  -1>(), double upper = details::highestRealVMax< uint8_t,  -1>(), Overflow ovfBx = Ovf::forbidden > using   u8qm1 = q< uint8_t,  -1, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< uint8_t,   0>(), double upper = details::highestRealVMax< uint8_t,   0>(), Overflow ovfBx = Ovf::forbidden > using    u8q0 = q< uint8_t,   0, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< uint8_t,   1>(), double upper = details::highestRealVMax< uint8_t,   1>(), Overflow ovfBx = Ovf::forbidden > using    u8q1 = q< uint8_t,   1, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< uint8_t,   2>(), double upper = details::highestRealVMax< uint8_t,   2>(), Overflow ovfBx = Ovf::forbidden > using    u8q2 = q< uint8_t,   2, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< uint8_t,   3>(), double upper = details::highestRealVMax< uint8_t,   3>(), Overflow ovfBx = Ovf::forbidden > using    u8q3 = q< uint8_t,   3, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< uint8_t,   4>(), double upper = details::highestRealVMax< uint8_t,   4>(), Overflow ovfBx = Ovf::forbidden > using    u8q4 = q< uint8_t,   4, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< uint8_t,   5>(), double upper = details::highestRealVMax< uint8_t,   5>(), Overflow ovfBx = Ovf::forbidden > using    u8q5 = q< uint8_t,   5, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< uint8_t,   6>(), double upper = details::highestRealVMax< uint8_t,   6>(), Overflow ovfBx = Ovf::forbidden > using    u8q6 = q< uint8_t,   6, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< uint8_t,   7>(), double upper = details::highestRealVMax< uint8_t,   7>(), Overflow ovfBx = Ovf::forbidden > using    u8q7 = q< uint8_t,   7, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int16_t,  -8>(), double upper = details::highestRealVMax< int16_t,  -8>(), Overflow ovfBx = Ovf::forbidden > using  i16qm8 = q< int16_t,  -8, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int16_t,  -7>(), double upper = details::highestRealVMax< int16_t,  -7>(), Overflow ovfBx = Ovf::forbidden > using  i16qm7 = q< int16_t,  -7, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int16_t,  -6>(), double upper = details::highestRealVMax< int16_t,  -6>(), Overflow ovfBx = Ovf::forbidden > using  i16qm6 = q< int16_t,  -6, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int16_t,  -5>(), double upper = details::highestRealVMax< int16_t,  -5>(), Overflow ovfBx = Ovf::forbidden > using  i16qm5 = q< int16_t,  -5, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int16_t,  -4>(), double upper = details::highestRealVMax< int16_t,  -4>(), Overflow ovfBx = Ovf::forbidden > using  i16qm4 = q< int16_t,  -4, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int16_t,  -3>(), double upper = details::highestRealVMax< int16_t,  -3>(), Overflow ovfBx = Ovf::forbidden > using  i16qm3 = q< int16_t,  -3, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int16_t,  -2>(), double upper = details::highestRealVMax< int16_t,  -2>(), Overflow ovfBx = Ovf::forbidden > using  i16qm2 = q< int16_t,  -2, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int16_t,  -1>(), double upper = details::highestRealVMax< int16_t,  -1>(), Overflow ovfBx = Ovf::forbidden > using  i16qm1 = q< int16_t,  -1, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int16_t,   0>(), double upper = details::highestRealVMax< int16_t,   0>(), Overflow ovfBx = Ovf::forbidden > using   i16q0 = q< int16_t,   0, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int16_t,   1>(), double upper = details::highestRealVMax< int16_t,   1>(), Overflow ovfBx = Ovf::forbidden > using   i16q1 = q< int16_t,   1, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int16_t,   2>(), double upper = details::highestRealVMax< int16_t,   2>(), Overflow ovfBx = Ovf::forbidden > using   i16q2 = q< int16_t,   2, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int16_t,   3>(), double upper = details::highestRealVMax< int16_t,   3>(), Overflow ovfBx = Ovf::forbidden > using   i16q3 = q< int16_t,   3, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int16_t,   4>(), double upper = details::highestRealVMax< int16_t,   4>(), Overflow ovfBx = Ovf::forbidden > using   i16q4 = q< int16_t,   4, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int16_t,   5>(), double upper = details::highestRealVMax< int16_t,   5>(), Overflow ovfBx = Ovf::forbidden > using   i16q5 = q< int16_t,   5, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int16_t,   6>(), double upper = details::highestRealVMax< int16_t,   6>(), Overflow ovfBx = Ovf::forbidden > using   i16q6 = q< int16_t,   6, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int16_t,   7>(), double upper = details::highestRealVMax< int16_t,   7>(), Overflow ovfBx = Ovf::forbidden > using   i16q7 = q< int16_t,   7, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int16_t,   8>(), double upper = details::highestRealVMax< int16_t,   8>(), Overflow ovfBx = Ovf::forbidden > using   i16q8 = q< int16_t,   8, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int16_t,   9>(), double upper = details::highestRealVMax< int16_t,   9>(), Overflow ovfBx = Ovf::forbidden > using   i16q9 = q< int16_t,   9, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int16_t,  10>(), double upper = details::highestRealVMax< int16_t,  10>(), Overflow ovfBx = Ovf::forbidden > using  i16q10 = q< int16_t,  10, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int16_t,  11>(), double upper = details::highestRealVMax< int16_t,  11>(), Overflow ovfBx = Ovf::forbidden > using  i16q11 = q< int16_t,  11, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int16_t,  12>(), double upper = details::highestRealVMax< int16_t,  12>(), Overflow ovfBx = Ovf::forbidden > using  i16q12 = q< int16_t,  12, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int16_t,  13>(), double upper = details::highestRealVMax< int16_t,  13>(), Overflow ovfBx = Ovf::forbidden > using  i16q13 = q< int16_t,  13, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int16_t,  14>(), double upper = details::highestRealVMax< int16_t,  14>(), Overflow ovfBx = Ovf::forbidden > using  i16q14 = q< int16_t,  14, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int16_t,  15>(), double upper = details::highestRealVMax< int16_t,  15>(), Overflow ovfBx = Ovf::forbidden > using  i16q15 = q< int16_t,  15, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint16_t,  -8>(), double upper = details::highestRealVMax<uint16_t,  -8>(), Overflow ovfBx = Ovf::forbidden > using  u16qm8 = q<uint16_t,  -8, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint16_t,  -7>(), double upper = details::highestRealVMax<uint16_t,  -7>(), Overflow ovfBx = Ovf::forbidden > using  u16qm7 = q<uint16_t,  -7, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint16_t,  -6>(), double upper = details::highestRealVMax<uint16_t,  -6>(), Overflow ovfBx = Ovf::forbidden > using  u16qm6 = q<uint16_t,  -6, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint16_t,  -5>(), double upper = details::highestRealVMax<uint16_t,  -5>(), Overflow ovfBx = Ovf::forbidden > using  u16qm5 = q<uint16_t,  -5, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint16_t,  -4>(), double upper = details::highestRealVMax<uint16_t,  -4>(), Overflow ovfBx = Ovf::forbidden > using  u16qm4 = q<uint16_t,  -4, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint16_t,  -3>(), double upper = details::highestRealVMax<uint16_t,  -3>(), Overflow ovfBx = Ovf::forbidden > using  u16qm3 = q<uint16_t,  -3, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint16_t,  -2>(), double upper = details::highestRealVMax<uint16_t,  -2>(), Overflow ovfBx = Ovf::forbidden > using  u16qm2 = q<uint16_t,  -2, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint16_t,  -1>(), double upper = details::highestRealVMax<uint16_t,  -1>(), Overflow ovfBx = Ovf::forbidden > using  u16qm1 = q<uint16_t,  -1, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint16_t,   0>(), double upper = details::highestRealVMax<uint16_t,   0>(), Overflow ovfBx = Ovf::forbidden > using   u16q0 = q<uint16_t,   0, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint16_t,   1>(), double upper = details::highestRealVMax<uint16_t,   1>(), Overflow ovfBx = Ovf::forbidden > using   u16q1 = q<uint16_t,   1, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint16_t,   2>(), double upper = details::highestRealVMax<uint16_t,   2>(), Overflow ovfBx = Ovf::forbidden > using   u16q2 = q<uint16_t,   2, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint16_t,   3>(), double upper = details::highestRealVMax<uint16_t,   3>(), Overflow ovfBx = Ovf::forbidden > using   u16q3 = q<uint16_t,   3, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint16_t,   4>(), double upper = details::highestRealVMax<uint16_t,   4>(), Overflow ovfBx = Ovf::forbidden > using   u16q4 = q<uint16_t,   4, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint16_t,   5>(), double upper = details::highestRealVMax<uint16_t,   5>(), Overflow ovfBx = Ovf::forbidden > using   u16q5 = q<uint16_t,   5, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint16_t,   6>(), double upper = details::highestRealVMax<uint16_t,   6>(), Overflow ovfBx = Ovf::forbidden > using   u16q6 = q<uint16_t,   6, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint16_t,   7>(), double upper = details::highestRealVMax<uint16_t,   7>(), Overflow ovfBx = Ovf::forbidden > using   u16q7 = q<uint16_t,   7, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint16_t,   8>(), double upper = details::highestRealVMax<uint16_t,   8>(), Overflow ovfBx = Ovf::forbidden > using   u16q8 = q<uint16_t,   8, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint16_t,   9>(), double upper = details::highestRealVMax<uint16_t,   9>(), Overflow ovfBx = Ovf::forbidden > using   u16q9 = q<uint16_t,   9, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint16_t,  10>(), double upper = details::highestRealVMax<uint16_t,  10>(), Overflow ovfBx = Ovf::forbidden > using  u16q10 = q<uint16_t,  10, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint16_t,  11>(), double upper = details::highestRealVMax<uint16_t,  11>(), Overflow ovfBx = Ovf::forbidden > using  u16q11 = q<uint16_t,  11, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint16_t,  12>(), double upper = details::highestRealVMax<uint16_t,  12>(), Overflow ovfBx = Ovf::forbidden > using  u16q12 = q<uint16_t,  12, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint16_t,  13>(), double upper = details::highestRealVMax<uint16_t,  13>(), Overflow ovfBx = Ovf::forbidden > using  u16q13 = q<uint16_t,  13, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint16_t,  14>(), double upper = details::highestRealVMax<uint16_t,  14>(), Overflow ovfBx = Ovf::forbidden > using  u16q14 = q<uint16_t,  14, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint16_t,  15>(), double upper = details::highestRealVMax<uint16_t,  15>(), Overflow ovfBx = Ovf::forbidden > using  u16q15 = q<uint16_t,  15, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int32_t, -16>(), double upper = details::highestRealVMax< int32_t, -16>(), Overflow ovfBx = Ovf::forbidden > using i32qm16 = q< int32_t, -16, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int32_t, -15>(), double upper = details::highestRealVMax< int32_t, -15>(), Overflow ovfBx = Ovf::forbidden > using i32qm15 = q< int32_t, -15, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int32_t, -14>(), double upper = details::highestRealVMax< int32_t, -14>(), Overflow ovfBx = Ovf::forbidden > using i32qm14 = q< int32_t, -14, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int32_t, -13>(), double upper = details::highestRealVMax< int32_t, -13>(), Overflow ovfBx = Ovf::forbidden > using i32qm13 = q< int32_t, -13, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int32_t, -12>(), double upper = details::highestRealVMax< int32_t, -12>(), Overflow ovfBx = Ovf::forbidden > using i32qm12 = q< int32_t, -12, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int32_t, -11>(), double upper = details::highestRealVMax< int32_t, -11>(), Overflow ovfBx = Ovf::forbidden > using i32qm11 = q< int32_t, -11, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int32_t, -10>(), double upper = details::highestRealVMax< int32_t, -10>(), Overflow ovfBx = Ovf::forbidden > using i32qm10 = q< int32_t, -10, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int32_t,  -9>(), double upper = details::highestRealVMax< int32_t,  -9>(), Overflow ovfBx = Ovf::forbidden > using  i32qm9 = q< int32_t,  -9, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int32_t,  -8>(), double upper = details::highestRealVMax< int32_t,  -8>(), Overflow ovfBx = Ovf::forbidden > using  i32qm8 = q< int32_t,  -8, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int32_t,  -7>(), double upper = details::highestRealVMax< int32_t,  -7>(), Overflow ovfBx = Ovf::forbidden > using  i32qm7 = q< int32_t,  -7, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int32_t,  -6>(), double upper = details::highestRealVMax< int32_t,  -6>(), Overflow ovfBx = Ovf::forbidden > using  i32qm6 = q< int32_t,  -6, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int32_t,  -5>(), double upper = details::highestRealVMax< int32_t,  -5>(), Overflow ovfBx = Ovf::forbidden > using  i32qm5 = q< int32_t,  -5, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int32_t,  -4>(), double upper = details::highestRealVMax< int32_t,  -4>(), Overflow ovfBx = Ovf::forbidden > using  i32qm4 = q< int32_t,  -4, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int32_t,  -3>(), double upper = details::highestRealVMax< int32_t,  -3>(), Overflow ovfBx = Ovf::forbidden > using  i32qm3 = q< int32_t,  -3, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int32_t,  -2>(), double upper = details::highestRealVMax< int32_t,  -2>(), Overflow ovfBx = Ovf::forbidden > using  i32qm2 = q< int32_t,  -2, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int32_t,  -1>(), double upper = details::highestRealVMax< int32_t,  -1>(), Overflow ovfBx = Ovf::forbidden > using  i32qm1 = q< int32_t,  -1, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int32_t,   0>(), double upper = details::highestRealVMax< int32_t,   0>(), Overflow ovfBx = Ovf::forbidden > using   i32q0 = q< int32_t,   0, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int32_t,   1>(), double upper = details::highestRealVMax< int32_t,   1>(), Overflow ovfBx = Ovf::forbidden > using   i32q1 = q< int32_t,   1, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int32_t,   2>(), double upper = details::highestRealVMax< int32_t,   2>(), Overflow ovfBx = Ovf::forbidden > using   i32q2 = q< int32_t,   2, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int32_t,   3>(), double upper = details::highestRealVMax< int32_t,   3>(), Overflow ovfBx = Ovf::forbidden > using   i32q3 = q< int32_t,   3, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int32_t,   4>(), double upper = details::highestRealVMax< int32_t,   4>(), Overflow ovfBx = Ovf::forbidden > using   i32q4 = q< int32_t,   4, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int32_t,   5>(), double upper = details::highestRealVMax< int32_t,   5>(), Overflow ovfBx = Ovf::forbidden > using   i32q5 = q< int32_t,   5, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int32_t,   6>(), double upper = details::highestRealVMax< int32_t,   6>(), Overflow ovfBx = Ovf::forbidden > using   i32q6 = q< int32_t,   6, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int32_t,   7>(), double upper = details::highestRealVMax< int32_t,   7>(), Overflow ovfBx = Ovf::forbidden > using   i32q7 = q< int32_t,   7, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int32_t,   8>(), double upper = details::highestRealVMax< int32_t,   8>(), Overflow ovfBx = Ovf::forbidden > using   i32q8 = q< int32_t,   8, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int32_t,   9>(), double upper = details::highestRealVMax< int32_t,   9>(), Overflow ovfBx = Ovf::forbidden > using   i32q9 = q< int32_t,   9, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int32_t,  10>(), double upper = details::highestRealVMax< int32_t,  10>(), Overflow ovfBx = Ovf::forbidden > using  i32q10 = q< int32_t,  10, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int32_t,  11>(), double upper = details::highestRealVMax< int32_t,  11>(), Overflow ovfBx = Ovf::forbidden > using  i32q11 = q< int32_t,  11, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int32_t,  12>(), double upper = details::highestRealVMax< int32_t,  12>(), Overflow ovfBx = Ovf::forbidden > using  i32q12 = q< int32_t,  12, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int32_t,  13>(), double upper = details::highestRealVMax< int32_t,  13>(), Overflow ovfBx = Ovf::forbidden > using  i32q13 = q< int32_t,  13, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int32_t,  14>(), double upper = details::highestRealVMax< int32_t,  14>(), Overflow ovfBx = Ovf::forbidden > using  i32q14 = q< int32_t,  14, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int32_t,  15>(), double upper = details::highestRealVMax< int32_t,  15>(), Overflow ovfBx = Ovf::forbidden > using  i32q15 = q< int32_t,  15, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int32_t,  16>(), double upper = details::highestRealVMax< int32_t,  16>(), Overflow ovfBx = Ovf::forbidden > using  i32q16 = q< int32_t,  16, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int32_t,  17>(), double upper = details::highestRealVMax< int32_t,  17>(), Overflow ovfBx = Ovf::forbidden > using  i32q17 = q< int32_t,  17, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int32_t,  18>(), double upper = details::highestRealVMax< int32_t,  18>(), Overflow ovfBx = Ovf::forbidden > using  i32q18 = q< int32_t,  18, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int32_t,  19>(), double upper = details::highestRealVMax< int32_t,  19>(), Overflow ovfBx = Ovf::forbidden > using  i32q19 = q< int32_t,  19, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int32_t,  20>(), double upper = details::highestRealVMax< int32_t,  20>(), Overflow ovfBx = Ovf::forbidden > using  i32q20 = q< int32_t,  20, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int32_t,  21>(), double upper = details::highestRealVMax< int32_t,  21>(), Overflow ovfBx = Ovf::forbidden > using  i32q21 = q< int32_t,  21, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int32_t,  22>(), double upper = details::highestRealVMax< int32_t,  22>(), Overflow ovfBx = Ovf::forbidden > using  i32q22 = q< int32_t,  22, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int32_t,  23>(), double upper = details::highestRealVMax< int32_t,  23>(), Overflow ovfBx = Ovf::forbidden > using  i32q23 = q< int32_t,  23, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int32_t,  24>(), double upper = details::highestRealVMax< int32_t,  24>(), Overflow ovfBx = Ovf::forbidden > using  i32q24 = q< int32_t,  24, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int32_t,  25>(), double upper = details::highestRealVMax< int32_t,  25>(), Overflow ovfBx = Ovf::forbidden > using  i32q25 = q< int32_t,  25, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int32_t,  26>(), double upper = details::highestRealVMax< int32_t,  26>(), Overflow ovfBx = Ovf::forbidden > using  i32q26 = q< int32_t,  26, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int32_t,  27>(), double upper = details::highestRealVMax< int32_t,  27>(), Overflow ovfBx = Ovf::forbidden > using  i32q27 = q< int32_t,  27, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int32_t,  28>(), double upper = details::highestRealVMax< int32_t,  28>(), Overflow ovfBx = Ovf::forbidden > using  i32q28 = q< int32_t,  28, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int32_t,  29>(), double upper = details::highestRealVMax< int32_t,  29>(), Overflow ovfBx = Ovf::forbidden > using  i32q29 = q< int32_t,  29, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int32_t,  30>(), double upper = details::highestRealVMax< int32_t,  30>(), Overflow ovfBx = Ovf::forbidden > using  i32q30 = q< int32_t,  30, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin< int32_t,  31>(), double upper = details::highestRealVMax< int32_t,  31>(), Overflow ovfBx = Ovf::forbidden > using  i32q31 = q< int32_t,  31, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint32_t, -16>(), double upper = details::highestRealVMax<uint32_t, -16>(), Overflow ovfBx = Ovf::forbidden > using u32qm16 = q<uint32_t, -16, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint32_t, -15>(), double upper = details::highestRealVMax<uint32_t, -15>(), Overflow ovfBx = Ovf::forbidden > using u32qm15 = q<uint32_t, -15, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint32_t, -14>(), double upper = details::highestRealVMax<uint32_t, -14>(), Overflow ovfBx = Ovf::forbidden > using u32qm14 = q<uint32_t, -14, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint32_t, -13>(), double upper = details::highestRealVMax<uint32_t, -13>(), Overflow ovfBx = Ovf::forbidden > using u32qm13 = q<uint32_t, -13, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint32_t, -12>(), double upper = details::highestRealVMax<uint32_t, -12>(), Overflow ovfBx = Ovf::forbidden > using u32qm12 = q<uint32_t, -12, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint32_t, -11>(), double upper = details::highestRealVMax<uint32_t, -11>(), Overflow ovfBx = Ovf::forbidden > using u32qm11 = q<uint32_t, -11, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint32_t, -10>(), double upper = details::highestRealVMax<uint32_t, -10>(), Overflow ovfBx = Ovf::forbidden > using u32qm10 = q<uint32_t, -10, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint32_t,  -9>(), double upper = details::highestRealVMax<uint32_t,  -9>(), Overflow ovfBx = Ovf::forbidden > using  u32qm9 = q<uint32_t,  -9, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint32_t,  -8>(), double upper = details::highestRealVMax<uint32_t,  -8>(), Overflow ovfBx = Ovf::forbidden > using  u32qm8 = q<uint32_t,  -8, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint32_t,  -7>(), double upper = details::highestRealVMax<uint32_t,  -7>(), Overflow ovfBx = Ovf::forbidden > using  u32qm7 = q<uint32_t,  -7, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint32_t,  -6>(), double upper = details::highestRealVMax<uint32_t,  -6>(), Overflow ovfBx = Ovf::forbidden > using  u32qm6 = q<uint32_t,  -6, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint32_t,  -5>(), double upper = details::highestRealVMax<uint32_t,  -5>(), Overflow ovfBx = Ovf::forbidden > using  u32qm5 = q<uint32_t,  -5, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint32_t,  -4>(), double upper = details::highestRealVMax<uint32_t,  -4>(), Overflow ovfBx = Ovf::forbidden > using  u32qm4 = q<uint32_t,  -4, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint32_t,  -3>(), double upper = details::highestRealVMax<uint32_t,  -3>(), Overflow ovfBx = Ovf::forbidden > using  u32qm3 = q<uint32_t,  -3, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint32_t,  -2>(), double upper = details::highestRealVMax<uint32_t,  -2>(), Overflow ovfBx = Ovf::forbidden > using  u32qm2 = q<uint32_t,  -2, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint32_t,  -1>(), double upper = details::highestRealVMax<uint32_t,  -1>(), Overflow ovfBx = Ovf::forbidden > using  u32qm1 = q<uint32_t,  -1, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint32_t,   0>(), double upper = details::highestRealVMax<uint32_t,   0>(), Overflow ovfBx = Ovf::forbidden > using   u32q0 = q<uint32_t,   0, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint32_t,   1>(), double upper = details::highestRealVMax<uint32_t,   1>(), Overflow ovfBx = Ovf::forbidden > using   u32q1 = q<uint32_t,   1, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint32_t,   2>(), double upper = details::highestRealVMax<uint32_t,   2>(), Overflow ovfBx = Ovf::forbidden > using   u32q2 = q<uint32_t,   2, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint32_t,   3>(), double upper = details::highestRealVMax<uint32_t,   3>(), Overflow ovfBx = Ovf::forbidden > using   u32q3 = q<uint32_t,   3, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint32_t,   4>(), double upper = details::highestRealVMax<uint32_t,   4>(), Overflow ovfBx = Ovf::forbidden > using   u32q4 = q<uint32_t,   4, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint32_t,   5>(), double upper = details::highestRealVMax<uint32_t,   5>(), Overflow ovfBx = Ovf::forbidden > using   u32q5 = q<uint32_t,   5, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint32_t,   6>(), double upper = details::highestRealVMax<uint32_t,   6>(), Overflow ovfBx = Ovf::forbidden > using   u32q6 = q<uint32_t,   6, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint32_t,   7>(), double upper = details::highestRealVMax<uint32_t,   7>(), Overflow ovfBx = Ovf::forbidden > using   u32q7 = q<uint32_t,   7, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint32_t,   8>(), double upper = details::highestRealVMax<uint32_t,   8>(), Overflow ovfBx = Ovf::forbidden > using   u32q8 = q<uint32_t,   8, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint32_t,   9>(), double upper = details::highestRealVMax<uint32_t,   9>(), Overflow ovfBx = Ovf::forbidden > using   u32q9 = q<uint32_t,   9, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint32_t,  10>(), double upper = details::highestRealVMax<uint32_t,  10>(), Overflow ovfBx = Ovf::forbidden > using  u32q10 = q<uint32_t,  10, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint32_t,  11>(), double upper = details::highestRealVMax<uint32_t,  11>(), Overflow ovfBx = Ovf::forbidden > using  u32q11 = q<uint32_t,  11, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint32_t,  12>(), double upper = details::highestRealVMax<uint32_t,  12>(), Overflow ovfBx = Ovf::forbidden > using  u32q12 = q<uint32_t,  12, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint32_t,  13>(), double upper = details::highestRealVMax<uint32_t,  13>(), Overflow ovfBx = Ovf::forbidden > using  u32q13 = q<uint32_t,  13, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint32_t,  14>(), double upper = details::highestRealVMax<uint32_t,  14>(), Overflow ovfBx = Ovf::forbidden > using  u32q14 = q<uint32_t,  14, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint32_t,  15>(), double upper = details::highestRealVMax<uint32_t,  15>(), Overflow ovfBx = Ovf::forbidden > using  u32q15 = q<uint32_t,  15, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint32_t,  16>(), double upper = details::highestRealVMax<uint32_t,  16>(), Overflow ovfBx = Ovf::forbidden > using  u32q16 = q<uint32_t,  16, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint32_t,  17>(), double upper = details::highestRealVMax<uint32_t,  17>(), Overflow ovfBx = Ovf::forbidden > using  u32q17 = q<uint32_t,  17, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint32_t,  18>(), double upper = details::highestRealVMax<uint32_t,  18>(), Overflow ovfBx = Ovf::forbidden > using  u32q18 = q<uint32_t,  18, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint32_t,  19>(), double upper = details::highestRealVMax<uint32_t,  19>(), Overflow ovfBx = Ovf::forbidden > using  u32q19 = q<uint32_t,  19, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint32_t,  20>(), double upper = details::highestRealVMax<uint32_t,  20>(), Overflow ovfBx = Ovf::forbidden > using  u32q20 = q<uint32_t,  20, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint32_t,  21>(), double upper = details::highestRealVMax<uint32_t,  21>(), Overflow ovfBx = Ovf::forbidden > using  u32q21 = q<uint32_t,  21, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint32_t,  22>(), double upper = details::highestRealVMax<uint32_t,  22>(), Overflow ovfBx = Ovf::forbidden > using  u32q22 = q<uint32_t,  22, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint32_t,  23>(), double upper = details::highestRealVMax<uint32_t,  23>(), Overflow ovfBx = Ovf::forbidden > using  u32q23 = q<uint32_t,  23, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint32_t,  24>(), double upper = details::highestRealVMax<uint32_t,  24>(), Overflow ovfBx = Ovf::forbidden > using  u32q24 = q<uint32_t,  24, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint32_t,  25>(), double upper = details::highestRealVMax<uint32_t,  25>(), Overflow ovfBx = Ovf::forbidden > using  u32q25 = q<uint32_t,  25, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint32_t,  26>(), double upper = details::highestRealVMax<uint32_t,  26>(), Overflow ovfBx = Ovf::forbidden > using  u32q26 = q<uint32_t,  26, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint32_t,  27>(), double upper = details::highestRealVMax<uint32_t,  27>(), Overflow ovfBx = Ovf::forbidden > using  u32q27 = q<uint32_t,  27, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint32_t,  28>(), double upper = details::highestRealVMax<uint32_t,  28>(), Overflow ovfBx = Ovf::forbidden > using  u32q28 = q<uint32_t,  28, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint32_t,  29>(), double upper = details::highestRealVMax<uint32_t,  29>(), Overflow ovfBx = Ovf::forbidden > using  u32q29 = q<uint32_t,  29, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint32_t,  30>(), double upper = details::highestRealVMax<uint32_t,  30>(), Overflow ovfBx = Ovf::forbidden > using  u32q30 = q<uint32_t,  30, lower, upper, ovfBx>;
template< double lower = details::lowestRealVMin<uint32_t,  31>(), double upper = details::highestRealVMax<uint32_t,  31>(), Overflow ovfBx = Ovf::forbidden > using  u32q31 = q<uint32_t,  31, lower, upper, ovfBx>;

/* literal operators */

template< char ...chars > consteval auto operator""_i8qm4()   { return qFromLiteral<  i8qm4<>, chars...>(); }
template< char ...chars > consteval auto operator""_i8qm3()   { return qFromLiteral<  i8qm3<>, chars...>(); }
template< char ...chars > consteval auto operator""_i8qm2()   { return qFromLiteral<  i8qm2<>, chars...>(); }
template< char ...chars > consteval auto operator""_i8qm1()   { return qFromLiteral<  i8qm1<>, chars...>(); }
template< char ...chars > consteval auto operator""_i8q0()    { return qFromLiteral<   i8q0<>, chars...>(); }
template< char ...chars > consteval auto operator""_i8q1()    { return qFromLiteral<   i8q1<>, chars...>(); }
template< char ...chars > consteval auto operator""_i8q2()    { return qFromLiteral<   i8q2<>, chars...>(); }
template< char ...chars > consteval auto operator""_i8q3()    { return qFromLiteral<   i8q3<>, chars...>(); }
template< char ...chars > consteval auto operator""_i8q4()    { return qFromLiteral<   i8q4<>, chars...>(); }
template< char ...chars > consteval auto operator""_i8q5()    { return qFromLiteral<   i8q5<>, chars...>(); }
template< char ...chars > consteval auto operator""_i8q6()    { return qFromLiteral<   i8q6<>, chars...>(); }
template< char ...chars > consteval auto operator""_i8q7()    { return qFromLiteral<   i8q7<>, chars...>(); }
template< char ...chars > consteval auto operator""_u8qm4()   { return qFromLiteral<  u8qm4<>, chars...>(); }
template< char ...chars > consteval auto operator""_u8qm3()   { return qFromLiteral<  u8qm3<>, chars...>(); }
template< char ...chars > consteval auto operator""_u8qm2()   { return qFromLiteral<  u8qm2<>, chars...>(); }
template< char ...chars > consteval auto operator""_u8qm1()   { return qFromLiteral<  u8qm1<>, chars...>(); }
template< char ...chars > consteval auto operator""_u8q0()    { return qFromLiteral<   u8q0<>, chars...>(); }
template< char ...chars > consteval auto operator""_u8q1()    { return qFromLiteral<   u8q1<>, chars...>(); }
template< char ...chars > consteval auto operator""_u8q2()    { return qFromLiteral<   u8q2<>, chars...>(); }
template< char ...chars > consteval auto operator""_u8q3()    { return qFromLiteral<   u8q3<>, chars...>(); }
template< char ...chars > consteval auto operator""_u8q4()    { return qFromLiteral<   u8q4<>, chars...>(); }
template< char ...chars > consteval auto operator""_u8q5()    { return qFromLiteral<   u8q5<>, chars...>(); }
template< char ...chars > consteval auto operator""_u8q6()    { return qFromLiteral<   u8q6<>, chars...>(); }
template< char ...chars > consteval auto operator""_u8q7()    { return qFromLiteral<   u8q7<>, chars...>(); }
template< char ...chars > consteval auto operator""_i16qm8()  { return qFromLiteral< i16qm8<>, chars...>(); }
template< char ...chars > consteval auto operator""_i16qm7()  { return qFromLiteral< i16qm7<>, chars...>(); }
template< char ...chars > consteval auto operator""_i16qm6()  { return qFromLiteral< i16qm6<>, chars...>(); }
template< char ...chars > consteval auto operator""_i16qm5()  { return qFromLiteral< i16qm5<>, chars...>(); }
template< char ...chars > consteval auto operator""_i16qm4()  { return qFromLiteral< i16qm4<>, chars...>(); }
template< char ...chars > consteval auto operator""_i16qm3()  { return qFromLiteral< i16qm3<>, chars...>(); }
template< char ...chars > consteval auto operator""_i16qm2()  { return qFromLiteral< i16qm2<>, chars...>(); }
template< char ...chars > consteval auto operator""_i16qm1()  { return qFromLiteral< i16qm1<>, chars...>(); }
template< char ...chars > consteval auto operator""_i16q0()   { return qFromLiteral<  i16q0<>, chars...>(); }
template< char ...chars > consteval auto operator""_i16q1()   { return qFromLiteral<  i16q1<>, chars...>(); }
template< char ...chars > consteval auto operator""_i16q2()   { return qFromLiteral<  i16q2<>, chars...>(); }
template< char ...chars > consteval auto operator""_i16q3()   { return qFromLiteral<  i16q3<>, chars...>(); }
template< char ...chars > consteval auto operator""_i16q4()   { return qFromLiteral<  i16q4<>, chars...>(); }
template< char ...chars > consteval auto operator""_i16q5()   { return qFromLiteral<  i16q5<>, chars...>(); }
template< char ...chars > consteval auto operator""_i16q6()   { return qFromLiteral<  i16q6<>, chars...>(); }
template< char ...chars > consteval auto operator""_i16q7()   { return qFromLiteral<  i16q7<>, chars...>(); }
template< char ...chars > consteval auto operator""_i16q8()   { return qFromLiteral<  i16q8<>, chars...>(); }
template< char ...chars > consteval auto operator""_i16q9()   { return qFromLiteral<  i16q9<>, chars...>(); }
template< char ...chars > consteval auto operator""_i16q10()  { return qFromLiteral< i16q10<>, chars...>(); }
template< char ...chars > consteval auto operator""_i16q11()  { return qFromLiteral< i16q11<>, chars...>(); }
template< char ...chars > consteval auto operator""_i16q12()  { return qFromLiteral< i16q12<>, chars...>(); }
template< char ...chars > consteval auto operator""_i16q13()  { return qFromLiteral< i16q13<>, chars...>(); }
template< char ...chars > consteval auto operator""_i16q14()  { return qFromLiteral< i16q14<>, chars...>(); }
template< char ...chars > consteval auto operator""_i16q15()  { return qFromLiteral< i16q15<>, chars...>(); }
template< char ...chars > consteval auto operator""_u16qm8()  { return qFromLiteral< u16qm8<>, chars...>(); }
template< char ...chars > consteval auto operator""_u16qm7()  { return qFromLiteral< u16qm7<>, chars...>(); }
template< char ...chars > consteval auto operator""_u16qm6()  { return qFromLiteral< u16qm6<>, chars...>(); }
template< char ...chars > consteval auto operator""_u16qm5()  { return qFromLiteral< u16qm5<>, chars...>(); }
template< char ...chars > consteval auto operator""_u16qm4()  { return qFromLiteral< u16qm4<>, chars...>(); }
template< char ...chars > consteval auto operator""_u16qm3()  { return qFromLiteral< u16qm3<>, chars...>(); }
template< char ...chars > consteval auto operator""_u16qm2()  { return qFromLiteral< u16qm2<>, chars...>(); }
template< char ...chars > consteval auto operator""_u16qm1()  { return qFromLiteral< u16qm1<>, chars...>(); }
template< char ...chars > consteval auto operator""_u16q0()   { return qFromLiteral<  u16q0<>, chars...>(); }
template< char ...chars > consteval auto operator""_u16q1()   { return qFromLiteral<  u16q1<>, chars...>(); }
template< char ...chars > consteval auto operator""_u16q2()   { return qFromLiteral<  u16q2<>, chars...>(); }
template< char ...chars > consteval auto operator""_u16q3()   { return qFromLiteral<  u16q3<>, chars...>(); }
template< char ...chars > consteval auto operator""_u16q4()   { return qFromLiteral<  u16q4<>, chars...>(); }
template< char ...chars > consteval auto operator""_u16q5()   { return qFromLiteral<  u16q5<>, chars...>(); }
template< char ...chars > consteval auto operator""_u16q6()   { return qFromLiteral<  u16q6<>, chars...>(); }
template< char ...chars > consteval auto operator""_u16q7()   { return qFromLiteral<  u16q7<>, chars...>(); }
template< char ...chars > consteval auto operator""_u16q8()   { return qFromLiteral<  u16q8<>, chars...>(); }
template< char ...chars > consteval auto operator""_u16q9()   { return qFromLiteral<  u16q9<>, chars...>(); }
template< char ...chars > consteval auto operator""_u16q10()  { return qFromLiteral< u16q10<>, chars...>(); }
template< char ...chars > consteval auto operator""_u16q11()  { return qFromLiteral< u16q11<>, chars...>(); }
template< char ...chars > consteval auto operator""_u16q12()  { return qFromLiteral< u16q12<>, chars...>(); }
template< char ...chars > consteval auto operator""_u16q13()  { return qFromLiteral< u16q13<>, chars...>(); }
template< char ...chars > consteval auto operator""_u16q14()  { return qFromLiteral< u16q14<>, chars...>(); }
template< char ...chars > consteval auto operator""_u16q15()  { return qFromLiteral< u16q15<>, chars...>(); }
template< char ...chars > consteval auto operator""_i32qm16() { return qFromLiteral<i32qm16<>, chars...>(); }
template< char ...chars > consteval auto operator""_i32qm15() { return qFromLiteral<i32qm15<>, chars...>(); }
template< char ...chars > consteval auto operator""_i32qm14() { return qFromLiteral<i32qm14<>, chars...>(); }
template< char ...chars > consteval auto operator""_i32qm13() { return qFromLiteral<i32qm13<>, chars...>(); }
template< char ...chars > consteval auto operator""_i32qm12() { return qFromLiteral<i32qm12<>, chars...>(); }
template< char ...chars > consteval auto operator""_i32qm11() { return qFromLiteral<i32qm11<>, chars...>(); }
template< char ...chars > consteval auto operator""_i32qm10() { return qFromLiteral<i32qm10<>, chars...>(); }
template< char ...chars > consteval auto operator""_i32qm9()  { return qFromLiteral< i32qm9<>, chars...>(); }
template< char ...chars > consteval auto operator""_i32qm8()  { return qFromLiteral< i32qm8<>, chars...>(); }
template< char ...chars > consteval auto operator""_i32qm7()  { return qFromLiteral< i32qm7<>, chars...>(); }
template< char ...chars > consteval auto operator""_i32qm6()  { return qFromLiteral< i32qm6<>, chars...>(); }
template< char ...chars > consteval auto operator""_i32qm5()  { return qFromLiteral< i32qm5<>, chars...>(); }
template< char ...chars > consteval auto operator""_i32qm4()  { return qFromLiteral< i32qm4<>, chars...>(); }
template< char ...chars > consteval auto operator""_i32qm3()  { return qFromLiteral< i32qm3<>, chars...>(); }
template< char ...chars > consteval auto operator""_i32qm2()  { return qFromLiteral< i32qm2<>, chars...>(); }
template< char ...chars > consteval auto operator""_i32qm1()  { return qFromLiteral< i32qm1<>, chars...>(); }
template< char ...chars > consteval auto operator""_i32q0()   { return qFromLiteral<  i32q0<>, chars...>(); }
template< char ...chars > consteval auto operator""_i32q1()   { return qFromLiteral<  i32q1<>, chars...>(); }
template< char ...chars > consteval auto operator""_i32q2()   { return qFromLiteral<  i32q2<>, chars...>(); }
template< char ...chars > consteval auto operator""_i32q3()   { return qFromLiteral<  i32q3<>, chars...>(); }
template< char ...chars > consteval auto operator""_i32q4()   { return qFromLiteral<  i32q4<>, chars...>(); }
template< char ...chars > consteval auto operator""_i32q5()   { return qFromLiteral<  i32q5<>, chars...>(); }
template< char ...chars > consteval auto operator""_i32q6()   { return qFromLiteral<  i32q6<>, chars...>(); }
template< char ...chars > consteval auto operator""_i32q7()   { return qFromLiteral<  i32q7<>, chars...>(); }
template< char ...chars > consteval auto operator""_i32q8()   { return qFromLiteral<  i32q8<>, chars...>(); }
template< char ...chars > consteval auto operator""_i32q9()   { return qFromLiteral<  i32q9<>, chars...>(); }
template< char ...chars > consteval auto operator""_i32q10()  { return qFromLiteral< i32q10<>, chars...>(); }
template< char ...chars > consteval auto operator""_i32q11()  { return qFromLiteral< i32q11<>, chars...>(); }
template< char ...chars > consteval auto operator""_i32q12()  { return qFromLiteral< i32q12<>, chars...>(); }
template< char ...chars > consteval auto operator""_i32q13()  { return qFromLiteral< i32q13<>, chars...>(); }
template< char ...chars > consteval auto operator""_i32q14()  { return qFromLiteral< i32q14<>, chars...>(); }
template< char ...chars > consteval auto operator""_i32q15()  { return qFromLiteral< i32q15<>, chars...>(); }
template< char ...chars > consteval auto operator""_i32q16()  { return qFromLiteral< i32q16<>, chars...>(); }
template< char ...chars > consteval auto operator""_i32q17()  { return qFromLiteral< i32q17<>, chars...>(); }
template< char ...chars > consteval auto operator""_i32q18()  { return qFromLiteral< i32q18<>, chars...>(); }
template< char ...chars > consteval auto operator""_i32q19()  { return qFromLiteral< i32q19<>, chars...>(); }
template< char ...chars > consteval auto operator""_i32q20()  { return qFromLiteral< i32q20<>, chars...>(); }
template< char ...chars > consteval auto operator""_i32q21()  { return qFromLiteral< i32q21<>, chars...>(); }
template< char ...chars > consteval auto operator""_i32q22()  { return qFromLiteral< i32q22<>, chars...>(); }
template< char ...chars > consteval auto operator""_i32q23()  { return qFromLiteral< i32q23<>, chars...>(); }
template< char ...chars > consteval auto operator""_i32q24()  { return qFromLiteral< i32q24<>, chars...>(); }
template< char ...chars > consteval auto operator""_i32q25()  { return qFromLiteral< i32q25<>, chars...>(); }
template< char ...chars > consteval auto operator""_i32q26()  { return qFromLiteral< i32q26<>, chars...>(); }
template< char ...chars > consteval auto operator""_i32q27()  { return qFromLiteral< i32q27<>, chars...>(); }
template< char ...chars > consteval auto operator""_i32q28()  { return qFromLiteral< i32q28<>, chars...>(); }
template< char ...chars > consteval auto operator""_i32q29()  { return qFromLiteral< i32q29<>, chars...>(); }
template< char ...chars > consteval auto operator""_i32q30()  { return qFromLiteral< i32q30<>, chars...>(); }
template< char ...chars > consteval auto operator""_i32q31()  { return qFromLiteral< i32q31<>, chars...>(); }
template< char ...chars > consteval auto operator""_u32qm16() { return qFromLiteral<u32qm16<>, chars...>(); }
template< char ...chars > consteval auto operator""_u32qm15() { return qFromLiteral<u32qm15<>, chars...>(); }
template< char ...chars > consteval auto operator""_u32qm14() { return qFromLiteral<u32qm14<>, chars...>(); }
template< char ...chars > consteval auto operator""_u32qm13() { return qFromLiteral<u32qm13<>, chars...>(); }
template< char ...chars > consteval auto operator""_u32qm12() { return qFromLiteral<u32qm12<>, chars...>(); }
template< char ...chars > consteval auto operator""_u32qm11() { return qFromLiteral<u32qm11<>, chars...>(); }
template< char ...chars > consteval auto operator""_u32qm10() { return qFromLiteral<u32qm10<>, chars...>(); }
template< char ...chars > consteval auto operator""_u32qm9()  { return qFromLiteral< u32qm9<>, chars...>(); }
template< char ...chars > consteval auto operator""_u32qm8()  { return qFromLiteral< u32qm8<>, chars...>(); }
template< char ...chars > consteval auto operator""_u32qm7()  { return qFromLiteral< u32qm7<>, chars...>(); }
template< char ...chars > consteval auto operator""_u32qm6()  { return qFromLiteral< u32qm6<>, chars...>(); }
template< char ...chars > consteval auto operator""_u32qm5()  { return qFromLiteral< u32qm5<>, chars...>(); }
template< char ...chars > consteval auto operator""_u32qm4()  { return qFromLiteral< u32qm4<>, chars...>(); }
template< char ...chars > consteval auto operator""_u32qm3()  { return qFromLiteral< u32qm3<>, chars...>(); }
template< char ...chars > consteval auto operator""_u32qm2()  { return qFromLiteral< u32qm2<>, chars...>(); }
template< char ...chars > consteval auto operator""_u32qm1()  { return qFromLiteral< u32qm1<>, chars...>(); }
template< char ...chars > consteval auto operator""_u32q0()   { return qFromLiteral<  u32q0<>, chars...>(); }
template< char ...chars > consteval auto operator""_u32q1()   { return qFromLiteral<  u32q1<>, chars...>(); }
template< char ...chars > consteval auto operator""_u32q2()   { return qFromLiteral<  u32q2<>, chars...>(); }
template< char ...chars > consteval auto operator""_u32q3()   { return qFromLiteral<  u32q3<>, chars...>(); }
template< char ...chars > consteval auto operator""_u32q4()   { return qFromLiteral<  u32q4<>, chars...>(); }
template< char ...chars > consteval auto operator""_u32q5()   { return qFromLiteral<  u32q5<>, chars...>(); }
template< char ...chars > consteval auto operator""_u32q6()   { return qFromLiteral<  u32q6<>, chars...>(); }
template< char ...chars > consteval auto operator""_u32q7()   { return qFromLiteral<  u32q7<>, chars...>(); }
template< char ...chars > consteval auto operator""_u32q8()   { return qFromLiteral<  u32q8<>, chars...>(); }
template< char ...chars > consteval auto operator""_u32q9()   { return qFromLiteral<  u32q9<>, chars...>(); }
template< char ...chars > consteval auto operator""_u32q10()  { return qFromLiteral< u32q10<>, chars...>(); }
template< char ...chars > consteval auto operator""_u32q11()  { return qFromLiteral< u32q11<>, chars...>(); }
template< char ...chars > consteval auto operator""_u32q12()  { return qFromLiteral< u32q12<>, chars...>(); }
template< char ...chars > consteval auto operator""_u32q13()  { return qFromLiteral< u32q13<>, chars...>(); }
template< char ...chars > consteval auto operator""_u32q14()  { return qFromLiteral< u32q14<>, chars...>(); }
template< char ...chars > consteval auto operator""_u32q15()  { return qFromLiteral< u32q15<>, chars...>(); }
template< char ...chars > consteval auto operator""_u32q16()  { return qFromLiteral< u32q16<>, chars...>(); }
template< char ...chars > consteval auto operator""_u32q17()  { return qFromLiteral< u32q17<>, chars...>(); }
template< char ...chars > consteval auto operator""_u32q18()  { return qFromLiteral< u32q18<>, chars...>(); }
template< char ...chars > consteval auto operator""_u32q19()  { return qFromLiteral< u32q19<>, chars...>(); }
template< char ...chars > consteval auto operator""_u32q20()  { return qFromLiteral< u32q20<>, chars...>(); }
template< char ...chars > consteval auto operator""_u32q21()  { return qFromLiteral< u32q21<>, chars...>(); }
template< char ...chars > consteval auto operator""_u32q22()  { return qFromLiteral< u32q22<>, chars...>(); }
template< char ...chars > consteval auto operator""_u32q23()  { return qFromLiteral< u32q23<>, chars...>(); }
template< char ...chars > consteval auto operator""_u32q24()  { return qFromLiteral< u32q24<>, chars...>(); }
template< char ...chars > consteval auto operator""_u32q25()  { return qFromLiteral< u32q25<>, chars...>(); }
template< char ...chars > consteval auto operator""_u32q26()  { return qFromLiteral< u32q26<>, chars...>(); }
template< char ...chars > consteval auto operator""_u32q27()  { return qFromLiteral< u32q27<>, chars...>(); }
template< char ...chars > consteval auto operator""_u32q28()  { return qFromLiteral< u32q28<>, chars...>(); }
template< char ...chars > consteval auto operator""_u32q29()  { return qFromLiteral< u32q29<>, chars...>(); }
template< char ...chars > consteval auto operator""_u32q30()  { return qFromLiteral< u32q30<>, chars...>(); }
template< char ...chars > consteval auto operator""_u32q31()  { return qFromLiteral< u32q31<>, chars...>(); }

}

#endif
// EOF
