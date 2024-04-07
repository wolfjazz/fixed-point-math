/** \file
 * Declaration of the predefined q types and literals.
 */

#ifndef FPM_FPM_Q_TYPES_HPP_
#define FPM_FPM_Q_TYPES_HPP_

#include "q.hpp"

namespace fpm::q::inline types {
/** \addtogroup grp_fpmQ
 * \{ */

/* q types */

template< double lower = fpm::realMin<  int8_t,  -4>(), double upper = fpm::realMax<  int8_t,  -4>(), Overflow ovfBx = Ovf::forbidden > using   i8qm4 = Q<  int8_t,  -4, lower, upper, ovfBx>;
template< double lower = fpm::realMin<  int8_t,  -3>(), double upper = fpm::realMax<  int8_t,  -3>(), Overflow ovfBx = Ovf::forbidden > using   i8qm3 = Q<  int8_t,  -3, lower, upper, ovfBx>;
template< double lower = fpm::realMin<  int8_t,  -2>(), double upper = fpm::realMax<  int8_t,  -2>(), Overflow ovfBx = Ovf::forbidden > using   i8qm2 = Q<  int8_t,  -2, lower, upper, ovfBx>;
template< double lower = fpm::realMin<  int8_t,  -1>(), double upper = fpm::realMax<  int8_t,  -1>(), Overflow ovfBx = Ovf::forbidden > using   i8qm1 = Q<  int8_t,  -1, lower, upper, ovfBx>;
template< double lower = fpm::realMin<  int8_t,   0>(), double upper = fpm::realMax<  int8_t,   0>(), Overflow ovfBx = Ovf::forbidden > using    i8q0 = Q<  int8_t,   0, lower, upper, ovfBx>;
template< double lower = fpm::realMin<  int8_t,   1>(), double upper = fpm::realMax<  int8_t,   1>(), Overflow ovfBx = Ovf::forbidden > using    i8q1 = Q<  int8_t,   1, lower, upper, ovfBx>;
template< double lower = fpm::realMin<  int8_t,   2>(), double upper = fpm::realMax<  int8_t,   2>(), Overflow ovfBx = Ovf::forbidden > using    i8q2 = Q<  int8_t,   2, lower, upper, ovfBx>;
template< double lower = fpm::realMin<  int8_t,   3>(), double upper = fpm::realMax<  int8_t,   3>(), Overflow ovfBx = Ovf::forbidden > using    i8q3 = Q<  int8_t,   3, lower, upper, ovfBx>;
template< double lower = fpm::realMin<  int8_t,   4>(), double upper = fpm::realMax<  int8_t,   4>(), Overflow ovfBx = Ovf::forbidden > using    i8q4 = Q<  int8_t,   4, lower, upper, ovfBx>;
template< double lower = fpm::realMin<  int8_t,   5>(), double upper = fpm::realMax<  int8_t,   5>(), Overflow ovfBx = Ovf::forbidden > using    i8q5 = Q<  int8_t,   5, lower, upper, ovfBx>;
template< double lower = fpm::realMin<  int8_t,   6>(), double upper = fpm::realMax<  int8_t,   6>(), Overflow ovfBx = Ovf::forbidden > using    i8q6 = Q<  int8_t,   6, lower, upper, ovfBx>;
template< double lower = fpm::realMin<  int8_t,   7>(), double upper = fpm::realMax<  int8_t,   7>(), Overflow ovfBx = Ovf::forbidden > using    i8q7 = Q<  int8_t,   7, lower, upper, ovfBx>;
template< double lower = fpm::realMin< uint8_t,  -4>(), double upper = fpm::realMax< uint8_t,  -4>(), Overflow ovfBx = Ovf::forbidden > using   u8qm4 = Q< uint8_t,  -4, lower, upper, ovfBx>;
template< double lower = fpm::realMin< uint8_t,  -3>(), double upper = fpm::realMax< uint8_t,  -3>(), Overflow ovfBx = Ovf::forbidden > using   u8qm3 = Q< uint8_t,  -3, lower, upper, ovfBx>;
template< double lower = fpm::realMin< uint8_t,  -2>(), double upper = fpm::realMax< uint8_t,  -2>(), Overflow ovfBx = Ovf::forbidden > using   u8qm2 = Q< uint8_t,  -2, lower, upper, ovfBx>;
template< double lower = fpm::realMin< uint8_t,  -1>(), double upper = fpm::realMax< uint8_t,  -1>(), Overflow ovfBx = Ovf::forbidden > using   u8qm1 = Q< uint8_t,  -1, lower, upper, ovfBx>;
template< double lower = fpm::realMin< uint8_t,   0>(), double upper = fpm::realMax< uint8_t,   0>(), Overflow ovfBx = Ovf::forbidden > using    u8q0 = Q< uint8_t,   0, lower, upper, ovfBx>;
template< double lower = fpm::realMin< uint8_t,   1>(), double upper = fpm::realMax< uint8_t,   1>(), Overflow ovfBx = Ovf::forbidden > using    u8q1 = Q< uint8_t,   1, lower, upper, ovfBx>;
template< double lower = fpm::realMin< uint8_t,   2>(), double upper = fpm::realMax< uint8_t,   2>(), Overflow ovfBx = Ovf::forbidden > using    u8q2 = Q< uint8_t,   2, lower, upper, ovfBx>;
template< double lower = fpm::realMin< uint8_t,   3>(), double upper = fpm::realMax< uint8_t,   3>(), Overflow ovfBx = Ovf::forbidden > using    u8q3 = Q< uint8_t,   3, lower, upper, ovfBx>;
template< double lower = fpm::realMin< uint8_t,   4>(), double upper = fpm::realMax< uint8_t,   4>(), Overflow ovfBx = Ovf::forbidden > using    u8q4 = Q< uint8_t,   4, lower, upper, ovfBx>;
template< double lower = fpm::realMin< uint8_t,   5>(), double upper = fpm::realMax< uint8_t,   5>(), Overflow ovfBx = Ovf::forbidden > using    u8q5 = Q< uint8_t,   5, lower, upper, ovfBx>;
template< double lower = fpm::realMin< uint8_t,   6>(), double upper = fpm::realMax< uint8_t,   6>(), Overflow ovfBx = Ovf::forbidden > using    u8q6 = Q< uint8_t,   6, lower, upper, ovfBx>;
template< double lower = fpm::realMin< uint8_t,   7>(), double upper = fpm::realMax< uint8_t,   7>(), Overflow ovfBx = Ovf::forbidden > using    u8q7 = Q< uint8_t,   7, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int16_t,  -8>(), double upper = fpm::realMax< int16_t,  -8>(), Overflow ovfBx = Ovf::forbidden > using  i16qm8 = Q< int16_t,  -8, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int16_t,  -7>(), double upper = fpm::realMax< int16_t,  -7>(), Overflow ovfBx = Ovf::forbidden > using  i16qm7 = Q< int16_t,  -7, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int16_t,  -6>(), double upper = fpm::realMax< int16_t,  -6>(), Overflow ovfBx = Ovf::forbidden > using  i16qm6 = Q< int16_t,  -6, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int16_t,  -5>(), double upper = fpm::realMax< int16_t,  -5>(), Overflow ovfBx = Ovf::forbidden > using  i16qm5 = Q< int16_t,  -5, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int16_t,  -4>(), double upper = fpm::realMax< int16_t,  -4>(), Overflow ovfBx = Ovf::forbidden > using  i16qm4 = Q< int16_t,  -4, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int16_t,  -3>(), double upper = fpm::realMax< int16_t,  -3>(), Overflow ovfBx = Ovf::forbidden > using  i16qm3 = Q< int16_t,  -3, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int16_t,  -2>(), double upper = fpm::realMax< int16_t,  -2>(), Overflow ovfBx = Ovf::forbidden > using  i16qm2 = Q< int16_t,  -2, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int16_t,  -1>(), double upper = fpm::realMax< int16_t,  -1>(), Overflow ovfBx = Ovf::forbidden > using  i16qm1 = Q< int16_t,  -1, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int16_t,   0>(), double upper = fpm::realMax< int16_t,   0>(), Overflow ovfBx = Ovf::forbidden > using   i16q0 = Q< int16_t,   0, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int16_t,   1>(), double upper = fpm::realMax< int16_t,   1>(), Overflow ovfBx = Ovf::forbidden > using   i16q1 = Q< int16_t,   1, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int16_t,   2>(), double upper = fpm::realMax< int16_t,   2>(), Overflow ovfBx = Ovf::forbidden > using   i16q2 = Q< int16_t,   2, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int16_t,   3>(), double upper = fpm::realMax< int16_t,   3>(), Overflow ovfBx = Ovf::forbidden > using   i16q3 = Q< int16_t,   3, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int16_t,   4>(), double upper = fpm::realMax< int16_t,   4>(), Overflow ovfBx = Ovf::forbidden > using   i16q4 = Q< int16_t,   4, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int16_t,   5>(), double upper = fpm::realMax< int16_t,   5>(), Overflow ovfBx = Ovf::forbidden > using   i16q5 = Q< int16_t,   5, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int16_t,   6>(), double upper = fpm::realMax< int16_t,   6>(), Overflow ovfBx = Ovf::forbidden > using   i16q6 = Q< int16_t,   6, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int16_t,   7>(), double upper = fpm::realMax< int16_t,   7>(), Overflow ovfBx = Ovf::forbidden > using   i16q7 = Q< int16_t,   7, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int16_t,   8>(), double upper = fpm::realMax< int16_t,   8>(), Overflow ovfBx = Ovf::forbidden > using   i16q8 = Q< int16_t,   8, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int16_t,   9>(), double upper = fpm::realMax< int16_t,   9>(), Overflow ovfBx = Ovf::forbidden > using   i16q9 = Q< int16_t,   9, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int16_t,  10>(), double upper = fpm::realMax< int16_t,  10>(), Overflow ovfBx = Ovf::forbidden > using  i16q10 = Q< int16_t,  10, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int16_t,  11>(), double upper = fpm::realMax< int16_t,  11>(), Overflow ovfBx = Ovf::forbidden > using  i16q11 = Q< int16_t,  11, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int16_t,  12>(), double upper = fpm::realMax< int16_t,  12>(), Overflow ovfBx = Ovf::forbidden > using  i16q12 = Q< int16_t,  12, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int16_t,  13>(), double upper = fpm::realMax< int16_t,  13>(), Overflow ovfBx = Ovf::forbidden > using  i16q13 = Q< int16_t,  13, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int16_t,  14>(), double upper = fpm::realMax< int16_t,  14>(), Overflow ovfBx = Ovf::forbidden > using  i16q14 = Q< int16_t,  14, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int16_t,  15>(), double upper = fpm::realMax< int16_t,  15>(), Overflow ovfBx = Ovf::forbidden > using  i16q15 = Q< int16_t,  15, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint16_t,  -8>(), double upper = fpm::realMax<uint16_t,  -8>(), Overflow ovfBx = Ovf::forbidden > using  u16qm8 = Q<uint16_t,  -8, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint16_t,  -7>(), double upper = fpm::realMax<uint16_t,  -7>(), Overflow ovfBx = Ovf::forbidden > using  u16qm7 = Q<uint16_t,  -7, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint16_t,  -6>(), double upper = fpm::realMax<uint16_t,  -6>(), Overflow ovfBx = Ovf::forbidden > using  u16qm6 = Q<uint16_t,  -6, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint16_t,  -5>(), double upper = fpm::realMax<uint16_t,  -5>(), Overflow ovfBx = Ovf::forbidden > using  u16qm5 = Q<uint16_t,  -5, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint16_t,  -4>(), double upper = fpm::realMax<uint16_t,  -4>(), Overflow ovfBx = Ovf::forbidden > using  u16qm4 = Q<uint16_t,  -4, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint16_t,  -3>(), double upper = fpm::realMax<uint16_t,  -3>(), Overflow ovfBx = Ovf::forbidden > using  u16qm3 = Q<uint16_t,  -3, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint16_t,  -2>(), double upper = fpm::realMax<uint16_t,  -2>(), Overflow ovfBx = Ovf::forbidden > using  u16qm2 = Q<uint16_t,  -2, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint16_t,  -1>(), double upper = fpm::realMax<uint16_t,  -1>(), Overflow ovfBx = Ovf::forbidden > using  u16qm1 = Q<uint16_t,  -1, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint16_t,   0>(), double upper = fpm::realMax<uint16_t,   0>(), Overflow ovfBx = Ovf::forbidden > using   u16q0 = Q<uint16_t,   0, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint16_t,   1>(), double upper = fpm::realMax<uint16_t,   1>(), Overflow ovfBx = Ovf::forbidden > using   u16q1 = Q<uint16_t,   1, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint16_t,   2>(), double upper = fpm::realMax<uint16_t,   2>(), Overflow ovfBx = Ovf::forbidden > using   u16q2 = Q<uint16_t,   2, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint16_t,   3>(), double upper = fpm::realMax<uint16_t,   3>(), Overflow ovfBx = Ovf::forbidden > using   u16q3 = Q<uint16_t,   3, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint16_t,   4>(), double upper = fpm::realMax<uint16_t,   4>(), Overflow ovfBx = Ovf::forbidden > using   u16q4 = Q<uint16_t,   4, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint16_t,   5>(), double upper = fpm::realMax<uint16_t,   5>(), Overflow ovfBx = Ovf::forbidden > using   u16q5 = Q<uint16_t,   5, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint16_t,   6>(), double upper = fpm::realMax<uint16_t,   6>(), Overflow ovfBx = Ovf::forbidden > using   u16q6 = Q<uint16_t,   6, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint16_t,   7>(), double upper = fpm::realMax<uint16_t,   7>(), Overflow ovfBx = Ovf::forbidden > using   u16q7 = Q<uint16_t,   7, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint16_t,   8>(), double upper = fpm::realMax<uint16_t,   8>(), Overflow ovfBx = Ovf::forbidden > using   u16q8 = Q<uint16_t,   8, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint16_t,   9>(), double upper = fpm::realMax<uint16_t,   9>(), Overflow ovfBx = Ovf::forbidden > using   u16q9 = Q<uint16_t,   9, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint16_t,  10>(), double upper = fpm::realMax<uint16_t,  10>(), Overflow ovfBx = Ovf::forbidden > using  u16q10 = Q<uint16_t,  10, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint16_t,  11>(), double upper = fpm::realMax<uint16_t,  11>(), Overflow ovfBx = Ovf::forbidden > using  u16q11 = Q<uint16_t,  11, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint16_t,  12>(), double upper = fpm::realMax<uint16_t,  12>(), Overflow ovfBx = Ovf::forbidden > using  u16q12 = Q<uint16_t,  12, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint16_t,  13>(), double upper = fpm::realMax<uint16_t,  13>(), Overflow ovfBx = Ovf::forbidden > using  u16q13 = Q<uint16_t,  13, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint16_t,  14>(), double upper = fpm::realMax<uint16_t,  14>(), Overflow ovfBx = Ovf::forbidden > using  u16q14 = Q<uint16_t,  14, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint16_t,  15>(), double upper = fpm::realMax<uint16_t,  15>(), Overflow ovfBx = Ovf::forbidden > using  u16q15 = Q<uint16_t,  15, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int32_t, -16>(), double upper = fpm::realMax< int32_t, -16>(), Overflow ovfBx = Ovf::forbidden > using i32qm16 = Q< int32_t, -16, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int32_t, -15>(), double upper = fpm::realMax< int32_t, -15>(), Overflow ovfBx = Ovf::forbidden > using i32qm15 = Q< int32_t, -15, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int32_t, -14>(), double upper = fpm::realMax< int32_t, -14>(), Overflow ovfBx = Ovf::forbidden > using i32qm14 = Q< int32_t, -14, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int32_t, -13>(), double upper = fpm::realMax< int32_t, -13>(), Overflow ovfBx = Ovf::forbidden > using i32qm13 = Q< int32_t, -13, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int32_t, -12>(), double upper = fpm::realMax< int32_t, -12>(), Overflow ovfBx = Ovf::forbidden > using i32qm12 = Q< int32_t, -12, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int32_t, -11>(), double upper = fpm::realMax< int32_t, -11>(), Overflow ovfBx = Ovf::forbidden > using i32qm11 = Q< int32_t, -11, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int32_t, -10>(), double upper = fpm::realMax< int32_t, -10>(), Overflow ovfBx = Ovf::forbidden > using i32qm10 = Q< int32_t, -10, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int32_t,  -9>(), double upper = fpm::realMax< int32_t,  -9>(), Overflow ovfBx = Ovf::forbidden > using  i32qm9 = Q< int32_t,  -9, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int32_t,  -8>(), double upper = fpm::realMax< int32_t,  -8>(), Overflow ovfBx = Ovf::forbidden > using  i32qm8 = Q< int32_t,  -8, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int32_t,  -7>(), double upper = fpm::realMax< int32_t,  -7>(), Overflow ovfBx = Ovf::forbidden > using  i32qm7 = Q< int32_t,  -7, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int32_t,  -6>(), double upper = fpm::realMax< int32_t,  -6>(), Overflow ovfBx = Ovf::forbidden > using  i32qm6 = Q< int32_t,  -6, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int32_t,  -5>(), double upper = fpm::realMax< int32_t,  -5>(), Overflow ovfBx = Ovf::forbidden > using  i32qm5 = Q< int32_t,  -5, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int32_t,  -4>(), double upper = fpm::realMax< int32_t,  -4>(), Overflow ovfBx = Ovf::forbidden > using  i32qm4 = Q< int32_t,  -4, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int32_t,  -3>(), double upper = fpm::realMax< int32_t,  -3>(), Overflow ovfBx = Ovf::forbidden > using  i32qm3 = Q< int32_t,  -3, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int32_t,  -2>(), double upper = fpm::realMax< int32_t,  -2>(), Overflow ovfBx = Ovf::forbidden > using  i32qm2 = Q< int32_t,  -2, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int32_t,  -1>(), double upper = fpm::realMax< int32_t,  -1>(), Overflow ovfBx = Ovf::forbidden > using  i32qm1 = Q< int32_t,  -1, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int32_t,   0>(), double upper = fpm::realMax< int32_t,   0>(), Overflow ovfBx = Ovf::forbidden > using   i32q0 = Q< int32_t,   0, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int32_t,   1>(), double upper = fpm::realMax< int32_t,   1>(), Overflow ovfBx = Ovf::forbidden > using   i32q1 = Q< int32_t,   1, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int32_t,   2>(), double upper = fpm::realMax< int32_t,   2>(), Overflow ovfBx = Ovf::forbidden > using   i32q2 = Q< int32_t,   2, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int32_t,   3>(), double upper = fpm::realMax< int32_t,   3>(), Overflow ovfBx = Ovf::forbidden > using   i32q3 = Q< int32_t,   3, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int32_t,   4>(), double upper = fpm::realMax< int32_t,   4>(), Overflow ovfBx = Ovf::forbidden > using   i32q4 = Q< int32_t,   4, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int32_t,   5>(), double upper = fpm::realMax< int32_t,   5>(), Overflow ovfBx = Ovf::forbidden > using   i32q5 = Q< int32_t,   5, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int32_t,   6>(), double upper = fpm::realMax< int32_t,   6>(), Overflow ovfBx = Ovf::forbidden > using   i32q6 = Q< int32_t,   6, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int32_t,   7>(), double upper = fpm::realMax< int32_t,   7>(), Overflow ovfBx = Ovf::forbidden > using   i32q7 = Q< int32_t,   7, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int32_t,   8>(), double upper = fpm::realMax< int32_t,   8>(), Overflow ovfBx = Ovf::forbidden > using   i32q8 = Q< int32_t,   8, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int32_t,   9>(), double upper = fpm::realMax< int32_t,   9>(), Overflow ovfBx = Ovf::forbidden > using   i32q9 = Q< int32_t,   9, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int32_t,  10>(), double upper = fpm::realMax< int32_t,  10>(), Overflow ovfBx = Ovf::forbidden > using  i32q10 = Q< int32_t,  10, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int32_t,  11>(), double upper = fpm::realMax< int32_t,  11>(), Overflow ovfBx = Ovf::forbidden > using  i32q11 = Q< int32_t,  11, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int32_t,  12>(), double upper = fpm::realMax< int32_t,  12>(), Overflow ovfBx = Ovf::forbidden > using  i32q12 = Q< int32_t,  12, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int32_t,  13>(), double upper = fpm::realMax< int32_t,  13>(), Overflow ovfBx = Ovf::forbidden > using  i32q13 = Q< int32_t,  13, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int32_t,  14>(), double upper = fpm::realMax< int32_t,  14>(), Overflow ovfBx = Ovf::forbidden > using  i32q14 = Q< int32_t,  14, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int32_t,  15>(), double upper = fpm::realMax< int32_t,  15>(), Overflow ovfBx = Ovf::forbidden > using  i32q15 = Q< int32_t,  15, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int32_t,  16>(), double upper = fpm::realMax< int32_t,  16>(), Overflow ovfBx = Ovf::forbidden > using  i32q16 = Q< int32_t,  16, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int32_t,  17>(), double upper = fpm::realMax< int32_t,  17>(), Overflow ovfBx = Ovf::forbidden > using  i32q17 = Q< int32_t,  17, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int32_t,  18>(), double upper = fpm::realMax< int32_t,  18>(), Overflow ovfBx = Ovf::forbidden > using  i32q18 = Q< int32_t,  18, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int32_t,  19>(), double upper = fpm::realMax< int32_t,  19>(), Overflow ovfBx = Ovf::forbidden > using  i32q19 = Q< int32_t,  19, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int32_t,  20>(), double upper = fpm::realMax< int32_t,  20>(), Overflow ovfBx = Ovf::forbidden > using  i32q20 = Q< int32_t,  20, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int32_t,  21>(), double upper = fpm::realMax< int32_t,  21>(), Overflow ovfBx = Ovf::forbidden > using  i32q21 = Q< int32_t,  21, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int32_t,  22>(), double upper = fpm::realMax< int32_t,  22>(), Overflow ovfBx = Ovf::forbidden > using  i32q22 = Q< int32_t,  22, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int32_t,  23>(), double upper = fpm::realMax< int32_t,  23>(), Overflow ovfBx = Ovf::forbidden > using  i32q23 = Q< int32_t,  23, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int32_t,  24>(), double upper = fpm::realMax< int32_t,  24>(), Overflow ovfBx = Ovf::forbidden > using  i32q24 = Q< int32_t,  24, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int32_t,  25>(), double upper = fpm::realMax< int32_t,  25>(), Overflow ovfBx = Ovf::forbidden > using  i32q25 = Q< int32_t,  25, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int32_t,  26>(), double upper = fpm::realMax< int32_t,  26>(), Overflow ovfBx = Ovf::forbidden > using  i32q26 = Q< int32_t,  26, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int32_t,  27>(), double upper = fpm::realMax< int32_t,  27>(), Overflow ovfBx = Ovf::forbidden > using  i32q27 = Q< int32_t,  27, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int32_t,  28>(), double upper = fpm::realMax< int32_t,  28>(), Overflow ovfBx = Ovf::forbidden > using  i32q28 = Q< int32_t,  28, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int32_t,  29>(), double upper = fpm::realMax< int32_t,  29>(), Overflow ovfBx = Ovf::forbidden > using  i32q29 = Q< int32_t,  29, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int32_t,  30>(), double upper = fpm::realMax< int32_t,  30>(), Overflow ovfBx = Ovf::forbidden > using  i32q30 = Q< int32_t,  30, lower, upper, ovfBx>;
template< double lower = fpm::realMin< int32_t,  31>(), double upper = fpm::realMax< int32_t,  31>(), Overflow ovfBx = Ovf::forbidden > using  i32q31 = Q< int32_t,  31, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint32_t, -16>(), double upper = fpm::realMax<uint32_t, -16>(), Overflow ovfBx = Ovf::forbidden > using u32qm16 = Q<uint32_t, -16, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint32_t, -15>(), double upper = fpm::realMax<uint32_t, -15>(), Overflow ovfBx = Ovf::forbidden > using u32qm15 = Q<uint32_t, -15, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint32_t, -14>(), double upper = fpm::realMax<uint32_t, -14>(), Overflow ovfBx = Ovf::forbidden > using u32qm14 = Q<uint32_t, -14, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint32_t, -13>(), double upper = fpm::realMax<uint32_t, -13>(), Overflow ovfBx = Ovf::forbidden > using u32qm13 = Q<uint32_t, -13, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint32_t, -12>(), double upper = fpm::realMax<uint32_t, -12>(), Overflow ovfBx = Ovf::forbidden > using u32qm12 = Q<uint32_t, -12, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint32_t, -11>(), double upper = fpm::realMax<uint32_t, -11>(), Overflow ovfBx = Ovf::forbidden > using u32qm11 = Q<uint32_t, -11, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint32_t, -10>(), double upper = fpm::realMax<uint32_t, -10>(), Overflow ovfBx = Ovf::forbidden > using u32qm10 = Q<uint32_t, -10, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint32_t,  -9>(), double upper = fpm::realMax<uint32_t,  -9>(), Overflow ovfBx = Ovf::forbidden > using  u32qm9 = Q<uint32_t,  -9, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint32_t,  -8>(), double upper = fpm::realMax<uint32_t,  -8>(), Overflow ovfBx = Ovf::forbidden > using  u32qm8 = Q<uint32_t,  -8, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint32_t,  -7>(), double upper = fpm::realMax<uint32_t,  -7>(), Overflow ovfBx = Ovf::forbidden > using  u32qm7 = Q<uint32_t,  -7, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint32_t,  -6>(), double upper = fpm::realMax<uint32_t,  -6>(), Overflow ovfBx = Ovf::forbidden > using  u32qm6 = Q<uint32_t,  -6, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint32_t,  -5>(), double upper = fpm::realMax<uint32_t,  -5>(), Overflow ovfBx = Ovf::forbidden > using  u32qm5 = Q<uint32_t,  -5, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint32_t,  -4>(), double upper = fpm::realMax<uint32_t,  -4>(), Overflow ovfBx = Ovf::forbidden > using  u32qm4 = Q<uint32_t,  -4, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint32_t,  -3>(), double upper = fpm::realMax<uint32_t,  -3>(), Overflow ovfBx = Ovf::forbidden > using  u32qm3 = Q<uint32_t,  -3, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint32_t,  -2>(), double upper = fpm::realMax<uint32_t,  -2>(), Overflow ovfBx = Ovf::forbidden > using  u32qm2 = Q<uint32_t,  -2, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint32_t,  -1>(), double upper = fpm::realMax<uint32_t,  -1>(), Overflow ovfBx = Ovf::forbidden > using  u32qm1 = Q<uint32_t,  -1, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint32_t,   0>(), double upper = fpm::realMax<uint32_t,   0>(), Overflow ovfBx = Ovf::forbidden > using   u32q0 = Q<uint32_t,   0, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint32_t,   1>(), double upper = fpm::realMax<uint32_t,   1>(), Overflow ovfBx = Ovf::forbidden > using   u32q1 = Q<uint32_t,   1, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint32_t,   2>(), double upper = fpm::realMax<uint32_t,   2>(), Overflow ovfBx = Ovf::forbidden > using   u32q2 = Q<uint32_t,   2, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint32_t,   3>(), double upper = fpm::realMax<uint32_t,   3>(), Overflow ovfBx = Ovf::forbidden > using   u32q3 = Q<uint32_t,   3, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint32_t,   4>(), double upper = fpm::realMax<uint32_t,   4>(), Overflow ovfBx = Ovf::forbidden > using   u32q4 = Q<uint32_t,   4, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint32_t,   5>(), double upper = fpm::realMax<uint32_t,   5>(), Overflow ovfBx = Ovf::forbidden > using   u32q5 = Q<uint32_t,   5, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint32_t,   6>(), double upper = fpm::realMax<uint32_t,   6>(), Overflow ovfBx = Ovf::forbidden > using   u32q6 = Q<uint32_t,   6, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint32_t,   7>(), double upper = fpm::realMax<uint32_t,   7>(), Overflow ovfBx = Ovf::forbidden > using   u32q7 = Q<uint32_t,   7, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint32_t,   8>(), double upper = fpm::realMax<uint32_t,   8>(), Overflow ovfBx = Ovf::forbidden > using   u32q8 = Q<uint32_t,   8, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint32_t,   9>(), double upper = fpm::realMax<uint32_t,   9>(), Overflow ovfBx = Ovf::forbidden > using   u32q9 = Q<uint32_t,   9, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint32_t,  10>(), double upper = fpm::realMax<uint32_t,  10>(), Overflow ovfBx = Ovf::forbidden > using  u32q10 = Q<uint32_t,  10, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint32_t,  11>(), double upper = fpm::realMax<uint32_t,  11>(), Overflow ovfBx = Ovf::forbidden > using  u32q11 = Q<uint32_t,  11, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint32_t,  12>(), double upper = fpm::realMax<uint32_t,  12>(), Overflow ovfBx = Ovf::forbidden > using  u32q12 = Q<uint32_t,  12, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint32_t,  13>(), double upper = fpm::realMax<uint32_t,  13>(), Overflow ovfBx = Ovf::forbidden > using  u32q13 = Q<uint32_t,  13, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint32_t,  14>(), double upper = fpm::realMax<uint32_t,  14>(), Overflow ovfBx = Ovf::forbidden > using  u32q14 = Q<uint32_t,  14, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint32_t,  15>(), double upper = fpm::realMax<uint32_t,  15>(), Overflow ovfBx = Ovf::forbidden > using  u32q15 = Q<uint32_t,  15, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint32_t,  16>(), double upper = fpm::realMax<uint32_t,  16>(), Overflow ovfBx = Ovf::forbidden > using  u32q16 = Q<uint32_t,  16, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint32_t,  17>(), double upper = fpm::realMax<uint32_t,  17>(), Overflow ovfBx = Ovf::forbidden > using  u32q17 = Q<uint32_t,  17, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint32_t,  18>(), double upper = fpm::realMax<uint32_t,  18>(), Overflow ovfBx = Ovf::forbidden > using  u32q18 = Q<uint32_t,  18, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint32_t,  19>(), double upper = fpm::realMax<uint32_t,  19>(), Overflow ovfBx = Ovf::forbidden > using  u32q19 = Q<uint32_t,  19, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint32_t,  20>(), double upper = fpm::realMax<uint32_t,  20>(), Overflow ovfBx = Ovf::forbidden > using  u32q20 = Q<uint32_t,  20, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint32_t,  21>(), double upper = fpm::realMax<uint32_t,  21>(), Overflow ovfBx = Ovf::forbidden > using  u32q21 = Q<uint32_t,  21, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint32_t,  22>(), double upper = fpm::realMax<uint32_t,  22>(), Overflow ovfBx = Ovf::forbidden > using  u32q22 = Q<uint32_t,  22, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint32_t,  23>(), double upper = fpm::realMax<uint32_t,  23>(), Overflow ovfBx = Ovf::forbidden > using  u32q23 = Q<uint32_t,  23, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint32_t,  24>(), double upper = fpm::realMax<uint32_t,  24>(), Overflow ovfBx = Ovf::forbidden > using  u32q24 = Q<uint32_t,  24, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint32_t,  25>(), double upper = fpm::realMax<uint32_t,  25>(), Overflow ovfBx = Ovf::forbidden > using  u32q25 = Q<uint32_t,  25, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint32_t,  26>(), double upper = fpm::realMax<uint32_t,  26>(), Overflow ovfBx = Ovf::forbidden > using  u32q26 = Q<uint32_t,  26, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint32_t,  27>(), double upper = fpm::realMax<uint32_t,  27>(), Overflow ovfBx = Ovf::forbidden > using  u32q27 = Q<uint32_t,  27, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint32_t,  28>(), double upper = fpm::realMax<uint32_t,  28>(), Overflow ovfBx = Ovf::forbidden > using  u32q28 = Q<uint32_t,  28, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint32_t,  29>(), double upper = fpm::realMax<uint32_t,  29>(), Overflow ovfBx = Ovf::forbidden > using  u32q29 = Q<uint32_t,  29, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint32_t,  30>(), double upper = fpm::realMax<uint32_t,  30>(), Overflow ovfBx = Ovf::forbidden > using  u32q30 = Q<uint32_t,  30, lower, upper, ovfBx>;
template< double lower = fpm::realMin<uint32_t,  31>(), double upper = fpm::realMax<uint32_t,  31>(), Overflow ovfBx = Ovf::forbidden > using  u32q31 = Q<uint32_t,  31, lower, upper, ovfBx>;

/* literal operators */

FPM_Q_BIND_LITERAL(  i8qm4<>, i8qm4  )
FPM_Q_BIND_LITERAL(  i8qm3<>, i8qm3  )
FPM_Q_BIND_LITERAL(  i8qm2<>, i8qm2  )
FPM_Q_BIND_LITERAL(  i8qm1<>, i8qm1  )
FPM_Q_BIND_LITERAL(   i8q0<>, i8q0   )
FPM_Q_BIND_LITERAL(   i8q1<>, i8q1   )
FPM_Q_BIND_LITERAL(   i8q2<>, i8q2   )
FPM_Q_BIND_LITERAL(   i8q3<>, i8q3   )
FPM_Q_BIND_LITERAL(   i8q4<>, i8q4   )
FPM_Q_BIND_LITERAL(   i8q5<>, i8q5   )
FPM_Q_BIND_LITERAL(   i8q6<>, i8q6   )
FPM_Q_BIND_LITERAL(   i8q7<>, i8q7   )
FPM_Q_BIND_LITERAL(  u8qm4<>, u8qm4  )
FPM_Q_BIND_LITERAL(  u8qm3<>, u8qm3  )
FPM_Q_BIND_LITERAL(  u8qm2<>, u8qm2  )
FPM_Q_BIND_LITERAL(  u8qm1<>, u8qm1  )
FPM_Q_BIND_LITERAL(   u8q0<>, u8q0   )
FPM_Q_BIND_LITERAL(   u8q1<>, u8q1   )
FPM_Q_BIND_LITERAL(   u8q2<>, u8q2   )
FPM_Q_BIND_LITERAL(   u8q3<>, u8q3   )
FPM_Q_BIND_LITERAL(   u8q4<>, u8q4   )
FPM_Q_BIND_LITERAL(   u8q5<>, u8q5   )
FPM_Q_BIND_LITERAL(   u8q6<>, u8q6   )
FPM_Q_BIND_LITERAL(   u8q7<>, u8q7   )
FPM_Q_BIND_LITERAL( i16qm8<>, i16qm8 )
FPM_Q_BIND_LITERAL( i16qm7<>, i16qm7 )
FPM_Q_BIND_LITERAL( i16qm6<>, i16qm6 )
FPM_Q_BIND_LITERAL( i16qm5<>, i16qm5 )
FPM_Q_BIND_LITERAL( i16qm4<>, i16qm4 )
FPM_Q_BIND_LITERAL( i16qm3<>, i16qm3 )
FPM_Q_BIND_LITERAL( i16qm2<>, i16qm2 )
FPM_Q_BIND_LITERAL( i16qm1<>, i16qm1 )
FPM_Q_BIND_LITERAL(  i16q0<>, i16q0  )
FPM_Q_BIND_LITERAL(  i16q1<>, i16q1  )
FPM_Q_BIND_LITERAL(  i16q2<>, i16q2  )
FPM_Q_BIND_LITERAL(  i16q3<>, i16q3  )
FPM_Q_BIND_LITERAL(  i16q4<>, i16q4  )
FPM_Q_BIND_LITERAL(  i16q5<>, i16q5  )
FPM_Q_BIND_LITERAL(  i16q6<>, i16q6  )
FPM_Q_BIND_LITERAL(  i16q7<>, i16q7  )
FPM_Q_BIND_LITERAL(  i16q8<>, i16q8  )
FPM_Q_BIND_LITERAL(  i16q9<>, i16q9  )
FPM_Q_BIND_LITERAL( i16q10<>, i16q10 )
FPM_Q_BIND_LITERAL( i16q11<>, i16q11 )
FPM_Q_BIND_LITERAL( i16q12<>, i16q12 )
FPM_Q_BIND_LITERAL( i16q13<>, i16q13 )
FPM_Q_BIND_LITERAL( i16q14<>, i16q14 )
FPM_Q_BIND_LITERAL( i16q15<>, i16q15 )
FPM_Q_BIND_LITERAL( u16qm8<>, u16qm8 )
FPM_Q_BIND_LITERAL( u16qm7<>, u16qm7 )
FPM_Q_BIND_LITERAL( u16qm6<>, u16qm6 )
FPM_Q_BIND_LITERAL( u16qm5<>, u16qm5 )
FPM_Q_BIND_LITERAL( u16qm4<>, u16qm4 )
FPM_Q_BIND_LITERAL( u16qm3<>, u16qm3 )
FPM_Q_BIND_LITERAL( u16qm2<>, u16qm2 )
FPM_Q_BIND_LITERAL( u16qm1<>, u16qm1 )
FPM_Q_BIND_LITERAL(  u16q0<>, u16q0  )
FPM_Q_BIND_LITERAL(  u16q1<>, u16q1  )
FPM_Q_BIND_LITERAL(  u16q2<>, u16q2  )
FPM_Q_BIND_LITERAL(  u16q3<>, u16q3  )
FPM_Q_BIND_LITERAL(  u16q4<>, u16q4  )
FPM_Q_BIND_LITERAL(  u16q5<>, u16q5  )
FPM_Q_BIND_LITERAL(  u16q6<>, u16q6  )
FPM_Q_BIND_LITERAL(  u16q7<>, u16q7  )
FPM_Q_BIND_LITERAL(  u16q8<>, u16q8  )
FPM_Q_BIND_LITERAL(  u16q9<>, u16q9  )
FPM_Q_BIND_LITERAL( u16q10<>, u16q10 )
FPM_Q_BIND_LITERAL( u16q11<>, u16q11 )
FPM_Q_BIND_LITERAL( u16q12<>, u16q12 )
FPM_Q_BIND_LITERAL( u16q13<>, u16q13 )
FPM_Q_BIND_LITERAL( u16q14<>, u16q14 )
FPM_Q_BIND_LITERAL( u16q15<>, u16q15 )
FPM_Q_BIND_LITERAL(i32qm16<>, i32qm16)
FPM_Q_BIND_LITERAL(i32qm15<>, i32qm15)
FPM_Q_BIND_LITERAL(i32qm14<>, i32qm14)
FPM_Q_BIND_LITERAL(i32qm13<>, i32qm13)
FPM_Q_BIND_LITERAL(i32qm12<>, i32qm12)
FPM_Q_BIND_LITERAL(i32qm11<>, i32qm11)
FPM_Q_BIND_LITERAL(i32qm10<>, i32qm10)
FPM_Q_BIND_LITERAL( i32qm9<>, i32qm9 )
FPM_Q_BIND_LITERAL( i32qm8<>, i32qm8 )
FPM_Q_BIND_LITERAL( i32qm7<>, i32qm7 )
FPM_Q_BIND_LITERAL( i32qm6<>, i32qm6 )
FPM_Q_BIND_LITERAL( i32qm5<>, i32qm5 )
FPM_Q_BIND_LITERAL( i32qm4<>, i32qm4 )
FPM_Q_BIND_LITERAL( i32qm3<>, i32qm3 )
FPM_Q_BIND_LITERAL( i32qm2<>, i32qm2 )
FPM_Q_BIND_LITERAL( i32qm1<>, i32qm1 )
FPM_Q_BIND_LITERAL(  i32q0<>, i32q0  )
FPM_Q_BIND_LITERAL(  i32q1<>, i32q1  )
FPM_Q_BIND_LITERAL(  i32q2<>, i32q2  )
FPM_Q_BIND_LITERAL(  i32q3<>, i32q3  )
FPM_Q_BIND_LITERAL(  i32q4<>, i32q4  )
FPM_Q_BIND_LITERAL(  i32q5<>, i32q5  )
FPM_Q_BIND_LITERAL(  i32q6<>, i32q6  )
FPM_Q_BIND_LITERAL(  i32q7<>, i32q7  )
FPM_Q_BIND_LITERAL(  i32q8<>, i32q8  )
FPM_Q_BIND_LITERAL(  i32q9<>, i32q9  )
FPM_Q_BIND_LITERAL( i32q10<>, i32q10 )
FPM_Q_BIND_LITERAL( i32q11<>, i32q11 )
FPM_Q_BIND_LITERAL( i32q12<>, i32q12 )
FPM_Q_BIND_LITERAL( i32q13<>, i32q13 )
FPM_Q_BIND_LITERAL( i32q14<>, i32q14 )
FPM_Q_BIND_LITERAL( i32q15<>, i32q15 )
FPM_Q_BIND_LITERAL( i32q16<>, i32q16 )
FPM_Q_BIND_LITERAL( i32q17<>, i32q17 )
FPM_Q_BIND_LITERAL( i32q18<>, i32q18 )
FPM_Q_BIND_LITERAL( i32q19<>, i32q19 )
FPM_Q_BIND_LITERAL( i32q20<>, i32q20 )
FPM_Q_BIND_LITERAL( i32q21<>, i32q21 )
FPM_Q_BIND_LITERAL( i32q22<>, i32q22 )
FPM_Q_BIND_LITERAL( i32q23<>, i32q23 )
FPM_Q_BIND_LITERAL( i32q24<>, i32q24 )
FPM_Q_BIND_LITERAL( i32q25<>, i32q25 )
FPM_Q_BIND_LITERAL( i32q26<>, i32q26 )
FPM_Q_BIND_LITERAL( i32q27<>, i32q27 )
FPM_Q_BIND_LITERAL( i32q28<>, i32q28 )
FPM_Q_BIND_LITERAL( i32q29<>, i32q29 )
FPM_Q_BIND_LITERAL( i32q30<>, i32q30 )
FPM_Q_BIND_LITERAL( i32q31<>, i32q31 )
FPM_Q_BIND_LITERAL(u32qm16<>, u32qm16)
FPM_Q_BIND_LITERAL(u32qm15<>, u32qm15)
FPM_Q_BIND_LITERAL(u32qm14<>, u32qm14)
FPM_Q_BIND_LITERAL(u32qm13<>, u32qm13)
FPM_Q_BIND_LITERAL(u32qm12<>, u32qm12)
FPM_Q_BIND_LITERAL(u32qm11<>, u32qm11)
FPM_Q_BIND_LITERAL(u32qm10<>, u32qm10)
FPM_Q_BIND_LITERAL( u32qm9<>, u32qm9 )
FPM_Q_BIND_LITERAL( u32qm8<>, u32qm8 )
FPM_Q_BIND_LITERAL( u32qm7<>, u32qm7 )
FPM_Q_BIND_LITERAL( u32qm6<>, u32qm6 )
FPM_Q_BIND_LITERAL( u32qm5<>, u32qm5 )
FPM_Q_BIND_LITERAL( u32qm4<>, u32qm4 )
FPM_Q_BIND_LITERAL( u32qm3<>, u32qm3 )
FPM_Q_BIND_LITERAL( u32qm2<>, u32qm2 )
FPM_Q_BIND_LITERAL( u32qm1<>, u32qm1 )
FPM_Q_BIND_LITERAL(  u32q0<>, u32q0  )
FPM_Q_BIND_LITERAL(  u32q1<>, u32q1  )
FPM_Q_BIND_LITERAL(  u32q2<>, u32q2  )
FPM_Q_BIND_LITERAL(  u32q3<>, u32q3  )
FPM_Q_BIND_LITERAL(  u32q4<>, u32q4  )
FPM_Q_BIND_LITERAL(  u32q5<>, u32q5  )
FPM_Q_BIND_LITERAL(  u32q6<>, u32q6  )
FPM_Q_BIND_LITERAL(  u32q7<>, u32q7  )
FPM_Q_BIND_LITERAL(  u32q8<>, u32q8  )
FPM_Q_BIND_LITERAL(  u32q9<>, u32q9  )
FPM_Q_BIND_LITERAL( u32q10<>, u32q10 )
FPM_Q_BIND_LITERAL( u32q11<>, u32q11 )
FPM_Q_BIND_LITERAL( u32q12<>, u32q12 )
FPM_Q_BIND_LITERAL( u32q13<>, u32q13 )
FPM_Q_BIND_LITERAL( u32q14<>, u32q14 )
FPM_Q_BIND_LITERAL( u32q15<>, u32q15 )
FPM_Q_BIND_LITERAL( u32q16<>, u32q16 )
FPM_Q_BIND_LITERAL( u32q17<>, u32q17 )
FPM_Q_BIND_LITERAL( u32q18<>, u32q18 )
FPM_Q_BIND_LITERAL( u32q19<>, u32q19 )
FPM_Q_BIND_LITERAL( u32q20<>, u32q20 )
FPM_Q_BIND_LITERAL( u32q21<>, u32q21 )
FPM_Q_BIND_LITERAL( u32q22<>, u32q22 )
FPM_Q_BIND_LITERAL( u32q23<>, u32q23 )
FPM_Q_BIND_LITERAL( u32q24<>, u32q24 )
FPM_Q_BIND_LITERAL( u32q25<>, u32q25 )
FPM_Q_BIND_LITERAL( u32q26<>, u32q26 )
FPM_Q_BIND_LITERAL( u32q27<>, u32q27 )
FPM_Q_BIND_LITERAL( u32q28<>, u32q28 )
FPM_Q_BIND_LITERAL( u32q29<>, u32q29 )
FPM_Q_BIND_LITERAL( u32q30<>, u32q30 )
FPM_Q_BIND_LITERAL( u32q31<>, u32q31 )

/**\}*/
}

#endif
// EOF
