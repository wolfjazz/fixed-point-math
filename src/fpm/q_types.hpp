/** \file
 * Declaration of the predefined q types and literals.
 */

#ifndef _FPM___Q_TYPES_HPP_
#define _FPM___Q_TYPES_HPP_

#include "q.hpp"

namespace fpm {

/* q types */

template< scaling_t f, double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  i8q = q<  int8_t, f, lower, upper, ovfBx>;
template< scaling_t f, double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  u8q = q< uint8_t, f, lower, upper, ovfBx>;
template< scaling_t f, double lower, double upper, Overflow ovfBx = Ovf::forbidden > using i16q = q< int16_t, f, lower, upper, ovfBx>;
template< scaling_t f, double lower, double upper, Overflow ovfBx = Ovf::forbidden > using u16q = q<uint16_t, f, lower, upper, ovfBx>;
template< scaling_t f, double lower, double upper, Overflow ovfBx = Ovf::forbidden > using i32q = q< int32_t, f, lower, upper, ovfBx>;
template< scaling_t f, double lower, double upper, Overflow ovfBx = Ovf::forbidden > using u32q = q<uint32_t, f, lower, upper, ovfBx>;

template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using   i8qm4 = q<  int8_t,  -4, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using   i8qm3 = q<  int8_t,  -3, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using   i8qm2 = q<  int8_t,  -2, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using   i8qm1 = q<  int8_t,  -1, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using    i8q0 = q<  int8_t,   0, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using    i8q1 = q<  int8_t,   1, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using    i8q2 = q<  int8_t,   2, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using    i8q3 = q<  int8_t,   3, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using    i8q4 = q<  int8_t,   4, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using    i8q5 = q<  int8_t,   5, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using    i8q6 = q<  int8_t,   6, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using    i8q7 = q<  int8_t,   7, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using   u8qm4 = q< uint8_t,  -4, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using   u8qm3 = q< uint8_t,  -3, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using   u8qm2 = q< uint8_t,  -2, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using   u8qm1 = q< uint8_t,  -1, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using    u8q0 = q< uint8_t,   0, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using    u8q1 = q< uint8_t,   1, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using    u8q2 = q< uint8_t,   2, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using    u8q3 = q< uint8_t,   3, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using    u8q4 = q< uint8_t,   4, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using    u8q5 = q< uint8_t,   5, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using    u8q6 = q< uint8_t,   6, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using    u8q7 = q< uint8_t,   7, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  i16qm8 = q< int16_t,  -8, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  i16qm7 = q< int16_t,  -7, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  i16qm6 = q< int16_t,  -6, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  i16qm5 = q< int16_t,  -5, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  i16qm4 = q< int16_t,  -4, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  i16qm3 = q< int16_t,  -3, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  i16qm2 = q< int16_t,  -2, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  i16qm1 = q< int16_t,  -1, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using   i16q0 = q< int16_t,   0, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using   i16q1 = q< int16_t,   1, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using   i16q2 = q< int16_t,   2, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using   i16q3 = q< int16_t,   3, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using   i16q4 = q< int16_t,   4, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using   i16q5 = q< int16_t,   5, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using   i16q6 = q< int16_t,   6, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using   i16q7 = q< int16_t,   7, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using   i16q8 = q< int16_t,   8, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using   i16q9 = q< int16_t,   9, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  i16q10 = q< int16_t,  10, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  i16q11 = q< int16_t,  11, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  i16q12 = q< int16_t,  12, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  i16q13 = q< int16_t,  13, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  i16q14 = q< int16_t,  14, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  i16q15 = q< int16_t,  15, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  u16qm8 = q<uint16_t,  -8, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  u16qm7 = q<uint16_t,  -7, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  u16qm6 = q<uint16_t,  -6, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  u16qm5 = q<uint16_t,  -5, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  u16qm4 = q<uint16_t,  -4, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  u16qm3 = q<uint16_t,  -3, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  u16qm2 = q<uint16_t,  -2, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  u16qm1 = q<uint16_t,  -1, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using   u16q0 = q<uint16_t,   0, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using   u16q1 = q<uint16_t,   1, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using   u16q2 = q<uint16_t,   2, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using   u16q3 = q<uint16_t,   3, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using   u16q4 = q<uint16_t,   4, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using   u16q5 = q<uint16_t,   5, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using   u16q6 = q<uint16_t,   6, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using   u16q7 = q<uint16_t,   7, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using   u16q8 = q<uint16_t,   8, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using   u16q9 = q<uint16_t,   9, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  u16q10 = q<uint16_t,  10, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  u16q11 = q<uint16_t,  11, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  u16q12 = q<uint16_t,  12, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  u16q13 = q<uint16_t,  13, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  u16q14 = q<uint16_t,  14, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  u16q15 = q<uint16_t,  15, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using i32qm16 = q< int32_t, -16, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using i32qm15 = q< int32_t, -15, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using i32qm14 = q< int32_t, -14, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using i32qm13 = q< int32_t, -13, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using i32qm12 = q< int32_t, -12, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using i32qm11 = q< int32_t, -11, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using i32qm10 = q< int32_t, -10, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  i32qm9 = q< int32_t,  -9, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  i32qm8 = q< int32_t,  -8, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  i32qm7 = q< int32_t,  -7, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  i32qm6 = q< int32_t,  -6, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  i32qm5 = q< int32_t,  -5, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  i32qm4 = q< int32_t,  -4, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  i32qm3 = q< int32_t,  -3, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  i32qm2 = q< int32_t,  -2, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  i32qm1 = q< int32_t,  -1, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using   i32q0 = q< int32_t,   0, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using   i32q1 = q< int32_t,   1, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using   i32q2 = q< int32_t,   2, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using   i32q3 = q< int32_t,   3, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using   i32q4 = q< int32_t,   4, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using   i32q5 = q< int32_t,   5, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using   i32q6 = q< int32_t,   6, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using   i32q7 = q< int32_t,   7, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using   i32q8 = q< int32_t,   8, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using   i32q9 = q< int32_t,   9, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  i32q10 = q< int32_t,  10, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  i32q11 = q< int32_t,  11, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  i32q12 = q< int32_t,  12, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  i32q13 = q< int32_t,  13, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  i32q14 = q< int32_t,  14, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  i32q15 = q< int32_t,  15, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  i32q16 = q< int32_t,  16, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  i32q17 = q< int32_t,  17, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  i32q18 = q< int32_t,  18, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  i32q19 = q< int32_t,  19, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  i32q20 = q< int32_t,  20, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  i32q21 = q< int32_t,  21, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  i32q22 = q< int32_t,  22, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  i32q23 = q< int32_t,  23, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  i32q24 = q< int32_t,  24, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  i32q25 = q< int32_t,  25, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  i32q26 = q< int32_t,  26, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  i32q27 = q< int32_t,  27, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  i32q28 = q< int32_t,  28, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  i32q29 = q< int32_t,  29, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  i32q30 = q< int32_t,  30, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  i32q31 = q< int32_t,  31, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using u32qm16 = q<uint32_t, -16, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using u32qm15 = q<uint32_t, -15, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using u32qm14 = q<uint32_t, -14, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using u32qm13 = q<uint32_t, -13, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using u32qm12 = q<uint32_t, -12, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using u32qm11 = q<uint32_t, -11, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using u32qm10 = q<uint32_t, -10, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  u32qm9 = q<uint32_t,  -9, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  u32qm8 = q<uint32_t,  -8, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  u32qm7 = q<uint32_t,  -7, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  u32qm6 = q<uint32_t,  -6, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  u32qm5 = q<uint32_t,  -5, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  u32qm4 = q<uint32_t,  -4, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  u32qm3 = q<uint32_t,  -3, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  u32qm2 = q<uint32_t,  -2, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  u32qm1 = q<uint32_t,  -1, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using   u32q0 = q<uint32_t,   0, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using   u32q1 = q<uint32_t,   1, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using   u32q2 = q<uint32_t,   2, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using   u32q3 = q<uint32_t,   3, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using   u32q4 = q<uint32_t,   4, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using   u32q5 = q<uint32_t,   5, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using   u32q6 = q<uint32_t,   6, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using   u32q7 = q<uint32_t,   7, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using   u32q8 = q<uint32_t,   8, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using   u32q9 = q<uint32_t,   9, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  u32q10 = q<uint32_t,  10, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  u32q11 = q<uint32_t,  11, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  u32q12 = q<uint32_t,  12, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  u32q13 = q<uint32_t,  13, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  u32q14 = q<uint32_t,  14, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  u32q15 = q<uint32_t,  15, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  u32q16 = q<uint32_t,  16, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  u32q17 = q<uint32_t,  17, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  u32q18 = q<uint32_t,  18, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  u32q19 = q<uint32_t,  19, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  u32q20 = q<uint32_t,  20, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  u32q21 = q<uint32_t,  21, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  u32q22 = q<uint32_t,  22, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  u32q23 = q<uint32_t,  23, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  u32q24 = q<uint32_t,  24, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  u32q25 = q<uint32_t,  25, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  u32q26 = q<uint32_t,  26, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  u32q27 = q<uint32_t,  27, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  u32q28 = q<uint32_t,  28, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  u32q29 = q<uint32_t,  29, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  u32q30 = q<uint32_t,  30, lower, upper, ovfBx>;
template< double lower, double upper, Overflow ovfBx = Ovf::forbidden > using  u32q31 = q<uint32_t,  31, lower, upper, ovfBx>;

/* literal operators */

template< char ...chars > consteval auto operator""_i8qm4()   { return qFromLiteral<  int8_t,  -4, chars...>(); }
template< char ...chars > consteval auto operator""_i8qm3()   { return qFromLiteral<  int8_t,  -3, chars...>(); }
template< char ...chars > consteval auto operator""_i8qm2()   { return qFromLiteral<  int8_t,  -2, chars...>(); }
template< char ...chars > consteval auto operator""_i8qm1()   { return qFromLiteral<  int8_t,  -1, chars...>(); }
template< char ...chars > consteval auto operator""_i8q0()    { return qFromLiteral<  int8_t,   0, chars...>(); }
template< char ...chars > consteval auto operator""_i8q1()    { return qFromLiteral<  int8_t,   1, chars...>(); }
template< char ...chars > consteval auto operator""_i8q2()    { return qFromLiteral<  int8_t,   2, chars...>(); }
template< char ...chars > consteval auto operator""_i8q3()    { return qFromLiteral<  int8_t,   3, chars...>(); }
template< char ...chars > consteval auto operator""_i8q4()    { return qFromLiteral<  int8_t,   4, chars...>(); }
template< char ...chars > consteval auto operator""_i8q5()    { return qFromLiteral<  int8_t,   5, chars...>(); }
template< char ...chars > consteval auto operator""_i8q6()    { return qFromLiteral<  int8_t,   6, chars...>(); }
template< char ...chars > consteval auto operator""_i8q7()    { return qFromLiteral<  int8_t,   7, chars...>(); }
template< char ...chars > consteval auto operator""_u8qm4()   { return qFromLiteral< uint8_t,  -4, chars...>(); }
template< char ...chars > consteval auto operator""_u8qm3()   { return qFromLiteral< uint8_t,  -3, chars...>(); }
template< char ...chars > consteval auto operator""_u8qm2()   { return qFromLiteral< uint8_t,  -2, chars...>(); }
template< char ...chars > consteval auto operator""_u8qm1()   { return qFromLiteral< uint8_t,  -1, chars...>(); }
template< char ...chars > consteval auto operator""_u8q0()    { return qFromLiteral< uint8_t,   0, chars...>(); }
template< char ...chars > consteval auto operator""_u8q1()    { return qFromLiteral< uint8_t,   1, chars...>(); }
template< char ...chars > consteval auto operator""_u8q2()    { return qFromLiteral< uint8_t,   2, chars...>(); }
template< char ...chars > consteval auto operator""_u8q3()    { return qFromLiteral< uint8_t,   3, chars...>(); }
template< char ...chars > consteval auto operator""_u8q4()    { return qFromLiteral< uint8_t,   4, chars...>(); }
template< char ...chars > consteval auto operator""_u8q5()    { return qFromLiteral< uint8_t,   5, chars...>(); }
template< char ...chars > consteval auto operator""_u8q6()    { return qFromLiteral< uint8_t,   6, chars...>(); }
template< char ...chars > consteval auto operator""_u8q7()    { return qFromLiteral< uint8_t,   7, chars...>(); }
template< char ...chars > consteval auto operator""_i16qm8()  { return qFromLiteral< int16_t,  -8, chars...>(); }
template< char ...chars > consteval auto operator""_i16qm7()  { return qFromLiteral< int16_t,  -7, chars...>(); }
template< char ...chars > consteval auto operator""_i16qm6()  { return qFromLiteral< int16_t,  -6, chars...>(); }
template< char ...chars > consteval auto operator""_i16qm5()  { return qFromLiteral< int16_t,  -5, chars...>(); }
template< char ...chars > consteval auto operator""_i16qm4()  { return qFromLiteral< int16_t,  -4, chars...>(); }
template< char ...chars > consteval auto operator""_i16qm3()  { return qFromLiteral< int16_t,  -3, chars...>(); }
template< char ...chars > consteval auto operator""_i16qm2()  { return qFromLiteral< int16_t,  -2, chars...>(); }
template< char ...chars > consteval auto operator""_i16qm1()  { return qFromLiteral< int16_t,  -1, chars...>(); }
template< char ...chars > consteval auto operator""_i16q0()   { return qFromLiteral< int16_t,   0, chars...>(); }
template< char ...chars > consteval auto operator""_i16q1()   { return qFromLiteral< int16_t,   1, chars...>(); }
template< char ...chars > consteval auto operator""_i16q2()   { return qFromLiteral< int16_t,   2, chars...>(); }
template< char ...chars > consteval auto operator""_i16q3()   { return qFromLiteral< int16_t,   3, chars...>(); }
template< char ...chars > consteval auto operator""_i16q4()   { return qFromLiteral< int16_t,   4, chars...>(); }
template< char ...chars > consteval auto operator""_i16q5()   { return qFromLiteral< int16_t,   5, chars...>(); }
template< char ...chars > consteval auto operator""_i16q6()   { return qFromLiteral< int16_t,   6, chars...>(); }
template< char ...chars > consteval auto operator""_i16q7()   { return qFromLiteral< int16_t,   7, chars...>(); }
template< char ...chars > consteval auto operator""_i16q8()   { return qFromLiteral< int16_t,   8, chars...>(); }
template< char ...chars > consteval auto operator""_i16q9()   { return qFromLiteral< int16_t,   9, chars...>(); }
template< char ...chars > consteval auto operator""_i16q10()  { return qFromLiteral< int16_t,  10, chars...>(); }
template< char ...chars > consteval auto operator""_i16q11()  { return qFromLiteral< int16_t,  11, chars...>(); }
template< char ...chars > consteval auto operator""_i16q12()  { return qFromLiteral< int16_t,  12, chars...>(); }
template< char ...chars > consteval auto operator""_i16q13()  { return qFromLiteral< int16_t,  13, chars...>(); }
template< char ...chars > consteval auto operator""_i16q14()  { return qFromLiteral< int16_t,  14, chars...>(); }
template< char ...chars > consteval auto operator""_i16q15()  { return qFromLiteral< int16_t,  15, chars...>(); }
template< char ...chars > consteval auto operator""_u16qm8()  { return qFromLiteral<uint16_t,  -8, chars...>(); }
template< char ...chars > consteval auto operator""_u16qm7()  { return qFromLiteral<uint16_t,  -7, chars...>(); }
template< char ...chars > consteval auto operator""_u16qm6()  { return qFromLiteral<uint16_t,  -6, chars...>(); }
template< char ...chars > consteval auto operator""_u16qm5()  { return qFromLiteral<uint16_t,  -5, chars...>(); }
template< char ...chars > consteval auto operator""_u16qm4()  { return qFromLiteral<uint16_t,  -4, chars...>(); }
template< char ...chars > consteval auto operator""_u16qm3()  { return qFromLiteral<uint16_t,  -3, chars...>(); }
template< char ...chars > consteval auto operator""_u16qm2()  { return qFromLiteral<uint16_t,  -2, chars...>(); }
template< char ...chars > consteval auto operator""_u16qm1()  { return qFromLiteral<uint16_t,  -1, chars...>(); }
template< char ...chars > consteval auto operator""_u16q0()   { return qFromLiteral<uint16_t,   0, chars...>(); }
template< char ...chars > consteval auto operator""_u16q1()   { return qFromLiteral<uint16_t,   1, chars...>(); }
template< char ...chars > consteval auto operator""_u16q2()   { return qFromLiteral<uint16_t,   2, chars...>(); }
template< char ...chars > consteval auto operator""_u16q3()   { return qFromLiteral<uint16_t,   3, chars...>(); }
template< char ...chars > consteval auto operator""_u16q4()   { return qFromLiteral<uint16_t,   4, chars...>(); }
template< char ...chars > consteval auto operator""_u16q5()   { return qFromLiteral<uint16_t,   5, chars...>(); }
template< char ...chars > consteval auto operator""_u16q6()   { return qFromLiteral<uint16_t,   6, chars...>(); }
template< char ...chars > consteval auto operator""_u16q7()   { return qFromLiteral<uint16_t,   7, chars...>(); }
template< char ...chars > consteval auto operator""_u16q8()   { return qFromLiteral<uint16_t,   8, chars...>(); }
template< char ...chars > consteval auto operator""_u16q9()   { return qFromLiteral<uint16_t,   9, chars...>(); }
template< char ...chars > consteval auto operator""_u16q10()  { return qFromLiteral<uint16_t,  10, chars...>(); }
template< char ...chars > consteval auto operator""_u16q11()  { return qFromLiteral<uint16_t,  11, chars...>(); }
template< char ...chars > consteval auto operator""_u16q12()  { return qFromLiteral<uint16_t,  12, chars...>(); }
template< char ...chars > consteval auto operator""_u16q13()  { return qFromLiteral<uint16_t,  13, chars...>(); }
template< char ...chars > consteval auto operator""_u16q14()  { return qFromLiteral<uint16_t,  14, chars...>(); }
template< char ...chars > consteval auto operator""_u16q15()  { return qFromLiteral<uint16_t,  15, chars...>(); }
template< char ...chars > consteval auto operator""_i32qm16() { return qFromLiteral< int32_t, -16, chars...>(); }
template< char ...chars > consteval auto operator""_i32qm15() { return qFromLiteral< int32_t, -15, chars...>(); }
template< char ...chars > consteval auto operator""_i32qm14() { return qFromLiteral< int32_t, -14, chars...>(); }
template< char ...chars > consteval auto operator""_i32qm13() { return qFromLiteral< int32_t, -13, chars...>(); }
template< char ...chars > consteval auto operator""_i32qm12() { return qFromLiteral< int32_t, -12, chars...>(); }
template< char ...chars > consteval auto operator""_i32qm11() { return qFromLiteral< int32_t, -11, chars...>(); }
template< char ...chars > consteval auto operator""_i32qm10() { return qFromLiteral< int32_t, -10, chars...>(); }
template< char ...chars > consteval auto operator""_i32qm9()  { return qFromLiteral< int32_t,  -9, chars...>(); }
template< char ...chars > consteval auto operator""_i32qm8()  { return qFromLiteral< int32_t,  -8, chars...>(); }
template< char ...chars > consteval auto operator""_i32qm7()  { return qFromLiteral< int32_t,  -7, chars...>(); }
template< char ...chars > consteval auto operator""_i32qm6()  { return qFromLiteral< int32_t,  -6, chars...>(); }
template< char ...chars > consteval auto operator""_i32qm5()  { return qFromLiteral< int32_t,  -5, chars...>(); }
template< char ...chars > consteval auto operator""_i32qm4()  { return qFromLiteral< int32_t,  -4, chars...>(); }
template< char ...chars > consteval auto operator""_i32qm3()  { return qFromLiteral< int32_t,  -3, chars...>(); }
template< char ...chars > consteval auto operator""_i32qm2()  { return qFromLiteral< int32_t,  -2, chars...>(); }
template< char ...chars > consteval auto operator""_i32qm1()  { return qFromLiteral< int32_t,  -1, chars...>(); }
template< char ...chars > consteval auto operator""_i32q0()   { return qFromLiteral< int32_t,   0, chars...>(); }
template< char ...chars > consteval auto operator""_i32q1()   { return qFromLiteral< int32_t,   1, chars...>(); }
template< char ...chars > consteval auto operator""_i32q2()   { return qFromLiteral< int32_t,   2, chars...>(); }
template< char ...chars > consteval auto operator""_i32q3()   { return qFromLiteral< int32_t,   3, chars...>(); }
template< char ...chars > consteval auto operator""_i32q4()   { return qFromLiteral< int32_t,   4, chars...>(); }
template< char ...chars > consteval auto operator""_i32q5()   { return qFromLiteral< int32_t,   5, chars...>(); }
template< char ...chars > consteval auto operator""_i32q6()   { return qFromLiteral< int32_t,   6, chars...>(); }
template< char ...chars > consteval auto operator""_i32q7()   { return qFromLiteral< int32_t,   7, chars...>(); }
template< char ...chars > consteval auto operator""_i32q8()   { return qFromLiteral< int32_t,   8, chars...>(); }
template< char ...chars > consteval auto operator""_i32q9()   { return qFromLiteral< int32_t,   9, chars...>(); }
template< char ...chars > consteval auto operator""_i32q10()  { return qFromLiteral< int32_t,  10, chars...>(); }
template< char ...chars > consteval auto operator""_i32q11()  { return qFromLiteral< int32_t,  11, chars...>(); }
template< char ...chars > consteval auto operator""_i32q12()  { return qFromLiteral< int32_t,  12, chars...>(); }
template< char ...chars > consteval auto operator""_i32q13()  { return qFromLiteral< int32_t,  13, chars...>(); }
template< char ...chars > consteval auto operator""_i32q14()  { return qFromLiteral< int32_t,  14, chars...>(); }
template< char ...chars > consteval auto operator""_i32q15()  { return qFromLiteral< int32_t,  15, chars...>(); }
template< char ...chars > consteval auto operator""_i32q16()  { return qFromLiteral< int32_t,  16, chars...>(); }
template< char ...chars > consteval auto operator""_i32q17()  { return qFromLiteral< int32_t,  17, chars...>(); }
template< char ...chars > consteval auto operator""_i32q18()  { return qFromLiteral< int32_t,  18, chars...>(); }
template< char ...chars > consteval auto operator""_i32q19()  { return qFromLiteral< int32_t,  19, chars...>(); }
template< char ...chars > consteval auto operator""_i32q20()  { return qFromLiteral< int32_t,  20, chars...>(); }
template< char ...chars > consteval auto operator""_i32q21()  { return qFromLiteral< int32_t,  21, chars...>(); }
template< char ...chars > consteval auto operator""_i32q22()  { return qFromLiteral< int32_t,  22, chars...>(); }
template< char ...chars > consteval auto operator""_i32q23()  { return qFromLiteral< int32_t,  23, chars...>(); }
template< char ...chars > consteval auto operator""_i32q24()  { return qFromLiteral< int32_t,  24, chars...>(); }
template< char ...chars > consteval auto operator""_i32q25()  { return qFromLiteral< int32_t,  25, chars...>(); }
template< char ...chars > consteval auto operator""_i32q26()  { return qFromLiteral< int32_t,  26, chars...>(); }
template< char ...chars > consteval auto operator""_i32q27()  { return qFromLiteral< int32_t,  27, chars...>(); }
template< char ...chars > consteval auto operator""_i32q28()  { return qFromLiteral< int32_t,  28, chars...>(); }
template< char ...chars > consteval auto operator""_i32q29()  { return qFromLiteral< int32_t,  29, chars...>(); }
template< char ...chars > consteval auto operator""_i32q30()  { return qFromLiteral< int32_t,  30, chars...>(); }
template< char ...chars > consteval auto operator""_i32q31()  { return qFromLiteral< int32_t,  31, chars...>(); }
template< char ...chars > consteval auto operator""_u32qm16() { return qFromLiteral<uint32_t, -16, chars...>(); }
template< char ...chars > consteval auto operator""_u32qm15() { return qFromLiteral<uint32_t, -15, chars...>(); }
template< char ...chars > consteval auto operator""_u32qm14() { return qFromLiteral<uint32_t, -14, chars...>(); }
template< char ...chars > consteval auto operator""_u32qm13() { return qFromLiteral<uint32_t, -13, chars...>(); }
template< char ...chars > consteval auto operator""_u32qm12() { return qFromLiteral<uint32_t, -12, chars...>(); }
template< char ...chars > consteval auto operator""_u32qm11() { return qFromLiteral<uint32_t, -11, chars...>(); }
template< char ...chars > consteval auto operator""_u32qm10() { return qFromLiteral<uint32_t, -10, chars...>(); }
template< char ...chars > consteval auto operator""_u32qm9()  { return qFromLiteral<uint32_t,  -9, chars...>(); }
template< char ...chars > consteval auto operator""_u32qm8()  { return qFromLiteral<uint32_t,  -8, chars...>(); }
template< char ...chars > consteval auto operator""_u32qm7()  { return qFromLiteral<uint32_t,  -7, chars...>(); }
template< char ...chars > consteval auto operator""_u32qm6()  { return qFromLiteral<uint32_t,  -6, chars...>(); }
template< char ...chars > consteval auto operator""_u32qm5()  { return qFromLiteral<uint32_t,  -5, chars...>(); }
template< char ...chars > consteval auto operator""_u32qm4()  { return qFromLiteral<uint32_t,  -4, chars...>(); }
template< char ...chars > consteval auto operator""_u32qm3()  { return qFromLiteral<uint32_t,  -3, chars...>(); }
template< char ...chars > consteval auto operator""_u32qm2()  { return qFromLiteral<uint32_t,  -2, chars...>(); }
template< char ...chars > consteval auto operator""_u32qm1()  { return qFromLiteral<uint32_t,  -1, chars...>(); }
template< char ...chars > consteval auto operator""_u32q0()   { return qFromLiteral<uint32_t,   0, chars...>(); }
template< char ...chars > consteval auto operator""_u32q1()   { return qFromLiteral<uint32_t,   1, chars...>(); }
template< char ...chars > consteval auto operator""_u32q2()   { return qFromLiteral<uint32_t,   2, chars...>(); }
template< char ...chars > consteval auto operator""_u32q3()   { return qFromLiteral<uint32_t,   3, chars...>(); }
template< char ...chars > consteval auto operator""_u32q4()   { return qFromLiteral<uint32_t,   4, chars...>(); }
template< char ...chars > consteval auto operator""_u32q5()   { return qFromLiteral<uint32_t,   5, chars...>(); }
template< char ...chars > consteval auto operator""_u32q6()   { return qFromLiteral<uint32_t,   6, chars...>(); }
template< char ...chars > consteval auto operator""_u32q7()   { return qFromLiteral<uint32_t,   7, chars...>(); }
template< char ...chars > consteval auto operator""_u32q8()   { return qFromLiteral<uint32_t,   8, chars...>(); }
template< char ...chars > consteval auto operator""_u32q9()   { return qFromLiteral<uint32_t,   9, chars...>(); }
template< char ...chars > consteval auto operator""_u32q10()  { return qFromLiteral<uint32_t,  10, chars...>(); }
template< char ...chars > consteval auto operator""_u32q11()  { return qFromLiteral<uint32_t,  11, chars...>(); }
template< char ...chars > consteval auto operator""_u32q12()  { return qFromLiteral<uint32_t,  12, chars...>(); }
template< char ...chars > consteval auto operator""_u32q13()  { return qFromLiteral<uint32_t,  13, chars...>(); }
template< char ...chars > consteval auto operator""_u32q14()  { return qFromLiteral<uint32_t,  14, chars...>(); }
template< char ...chars > consteval auto operator""_u32q15()  { return qFromLiteral<uint32_t,  15, chars...>(); }
template< char ...chars > consteval auto operator""_u32q16()  { return qFromLiteral<uint32_t,  16, chars...>(); }
template< char ...chars > consteval auto operator""_u32q17()  { return qFromLiteral<uint32_t,  17, chars...>(); }
template< char ...chars > consteval auto operator""_u32q18()  { return qFromLiteral<uint32_t,  18, chars...>(); }
template< char ...chars > consteval auto operator""_u32q19()  { return qFromLiteral<uint32_t,  19, chars...>(); }
template< char ...chars > consteval auto operator""_u32q20()  { return qFromLiteral<uint32_t,  20, chars...>(); }
template< char ...chars > consteval auto operator""_u32q21()  { return qFromLiteral<uint32_t,  21, chars...>(); }
template< char ...chars > consteval auto operator""_u32q22()  { return qFromLiteral<uint32_t,  22, chars...>(); }
template< char ...chars > consteval auto operator""_u32q23()  { return qFromLiteral<uint32_t,  23, chars...>(); }
template< char ...chars > consteval auto operator""_u32q24()  { return qFromLiteral<uint32_t,  24, chars...>(); }
template< char ...chars > consteval auto operator""_u32q25()  { return qFromLiteral<uint32_t,  25, chars...>(); }
template< char ...chars > consteval auto operator""_u32q26()  { return qFromLiteral<uint32_t,  26, chars...>(); }
template< char ...chars > consteval auto operator""_u32q27()  { return qFromLiteral<uint32_t,  27, chars...>(); }
template< char ...chars > consteval auto operator""_u32q28()  { return qFromLiteral<uint32_t,  28, chars...>(); }
template< char ...chars > consteval auto operator""_u32q29()  { return qFromLiteral<uint32_t,  29, chars...>(); }
template< char ...chars > consteval auto operator""_u32q30()  { return qFromLiteral<uint32_t,  30, chars...>(); }
template< char ...chars > consteval auto operator""_u32q31()  { return qFromLiteral<uint32_t,  31, chars...>(); }

}

#endif
// EOF
