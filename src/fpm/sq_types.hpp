/** \file
 * Declaration of the predefined sq types and literals.
 */

#ifndef FPM_FPM_SQ_TYPES_HPP_
#define FPM_FPM_SQ_TYPES_HPP_

#include "sq.hpp"

namespace fpm {

/* sq types */

template< scaling_t f, double lower, double upper > using  i8sq = sq<  int8_t, f, lower, upper>;
template< scaling_t f, double lower, double upper > using  u8sq = sq< uint8_t, f, lower, upper>;
template< scaling_t f, double lower, double upper > using i16sq = sq< int16_t, f, lower, upper>;
template< scaling_t f, double lower, double upper > using u16sq = sq<uint16_t, f, lower, upper>;
template< scaling_t f, double lower, double upper > using i32sq = sq< int32_t, f, lower, upper>;
template< scaling_t f, double lower, double upper > using u32sq = sq<uint32_t, f, lower, upper>;

template< double lower, double upper > using   i8sqm4 = sq<  int8_t,  -4, lower, upper>;
template< double lower, double upper > using   i8sqm3 = sq<  int8_t,  -3, lower, upper>;
template< double lower, double upper > using   i8sqm2 = sq<  int8_t,  -2, lower, upper>;
template< double lower, double upper > using   i8sqm1 = sq<  int8_t,  -1, lower, upper>;
template< double lower, double upper > using    i8sq0 = sq<  int8_t,   0, lower, upper>;
template< double lower, double upper > using    i8sq1 = sq<  int8_t,   1, lower, upper>;
template< double lower, double upper > using    i8sq2 = sq<  int8_t,   2, lower, upper>;
template< double lower, double upper > using    i8sq3 = sq<  int8_t,   3, lower, upper>;
template< double lower, double upper > using    i8sq4 = sq<  int8_t,   4, lower, upper>;
template< double lower, double upper > using    i8sq5 = sq<  int8_t,   5, lower, upper>;
template< double lower, double upper > using    i8sq6 = sq<  int8_t,   6, lower, upper>;
template< double lower, double upper > using    i8sq7 = sq<  int8_t,   7, lower, upper>;
template< double lower, double upper > using   u8sqm4 = sq< uint8_t,  -4, lower, upper>;
template< double lower, double upper > using   u8sqm3 = sq< uint8_t,  -3, lower, upper>;
template< double lower, double upper > using   u8sqm2 = sq< uint8_t,  -2, lower, upper>;
template< double lower, double upper > using   u8sqm1 = sq< uint8_t,  -1, lower, upper>;
template< double lower, double upper > using    u8sq0 = sq< uint8_t,   0, lower, upper>;
template< double lower, double upper > using    u8sq1 = sq< uint8_t,   1, lower, upper>;
template< double lower, double upper > using    u8sq2 = sq< uint8_t,   2, lower, upper>;
template< double lower, double upper > using    u8sq3 = sq< uint8_t,   3, lower, upper>;
template< double lower, double upper > using    u8sq4 = sq< uint8_t,   4, lower, upper>;
template< double lower, double upper > using    u8sq5 = sq< uint8_t,   5, lower, upper>;
template< double lower, double upper > using    u8sq6 = sq< uint8_t,   6, lower, upper>;
template< double lower, double upper > using    u8sq7 = sq< uint8_t,   7, lower, upper>;
template< double lower, double upper > using  i16sqm8 = sq< int16_t,  -8, lower, upper>;
template< double lower, double upper > using  i16sqm7 = sq< int16_t,  -7, lower, upper>;
template< double lower, double upper > using  i16sqm6 = sq< int16_t,  -6, lower, upper>;
template< double lower, double upper > using  i16sqm5 = sq< int16_t,  -5, lower, upper>;
template< double lower, double upper > using  i16sqm4 = sq< int16_t,  -4, lower, upper>;
template< double lower, double upper > using  i16sqm3 = sq< int16_t,  -3, lower, upper>;
template< double lower, double upper > using  i16sqm2 = sq< int16_t,  -2, lower, upper>;
template< double lower, double upper > using  i16sqm1 = sq< int16_t,  -1, lower, upper>;
template< double lower, double upper > using   i16sq0 = sq< int16_t,   0, lower, upper>;
template< double lower, double upper > using   i16sq1 = sq< int16_t,   1, lower, upper>;
template< double lower, double upper > using   i16sq2 = sq< int16_t,   2, lower, upper>;
template< double lower, double upper > using   i16sq3 = sq< int16_t,   3, lower, upper>;
template< double lower, double upper > using   i16sq4 = sq< int16_t,   4, lower, upper>;
template< double lower, double upper > using   i16sq5 = sq< int16_t,   5, lower, upper>;
template< double lower, double upper > using   i16sq6 = sq< int16_t,   6, lower, upper>;
template< double lower, double upper > using   i16sq7 = sq< int16_t,   7, lower, upper>;
template< double lower, double upper > using   i16sq8 = sq< int16_t,   8, lower, upper>;
template< double lower, double upper > using   i16sq9 = sq< int16_t,   9, lower, upper>;
template< double lower, double upper > using  i16sq10 = sq< int16_t,  10, lower, upper>;
template< double lower, double upper > using  i16sq11 = sq< int16_t,  11, lower, upper>;
template< double lower, double upper > using  i16sq12 = sq< int16_t,  12, lower, upper>;
template< double lower, double upper > using  i16sq13 = sq< int16_t,  13, lower, upper>;
template< double lower, double upper > using  i16sq14 = sq< int16_t,  14, lower, upper>;
template< double lower, double upper > using  i16sq15 = sq< int16_t,  15, lower, upper>;
template< double lower, double upper > using  u16sqm8 = sq<uint16_t,  -8, lower, upper>;
template< double lower, double upper > using  u16sqm7 = sq<uint16_t,  -7, lower, upper>;
template< double lower, double upper > using  u16sqm6 = sq<uint16_t,  -6, lower, upper>;
template< double lower, double upper > using  u16sqm5 = sq<uint16_t,  -5, lower, upper>;
template< double lower, double upper > using  u16sqm4 = sq<uint16_t,  -4, lower, upper>;
template< double lower, double upper > using  u16sqm3 = sq<uint16_t,  -3, lower, upper>;
template< double lower, double upper > using  u16sqm2 = sq<uint16_t,  -2, lower, upper>;
template< double lower, double upper > using  u16sqm1 = sq<uint16_t,  -1, lower, upper>;
template< double lower, double upper > using   u16sq0 = sq<uint16_t,   0, lower, upper>;
template< double lower, double upper > using   u16sq1 = sq<uint16_t,   1, lower, upper>;
template< double lower, double upper > using   u16sq2 = sq<uint16_t,   2, lower, upper>;
template< double lower, double upper > using   u16sq3 = sq<uint16_t,   3, lower, upper>;
template< double lower, double upper > using   u16sq4 = sq<uint16_t,   4, lower, upper>;
template< double lower, double upper > using   u16sq5 = sq<uint16_t,   5, lower, upper>;
template< double lower, double upper > using   u16sq6 = sq<uint16_t,   6, lower, upper>;
template< double lower, double upper > using   u16sq7 = sq<uint16_t,   7, lower, upper>;
template< double lower, double upper > using   u16sq8 = sq<uint16_t,   8, lower, upper>;
template< double lower, double upper > using   u16sq9 = sq<uint16_t,   9, lower, upper>;
template< double lower, double upper > using  u16sq10 = sq<uint16_t,  10, lower, upper>;
template< double lower, double upper > using  u16sq11 = sq<uint16_t,  11, lower, upper>;
template< double lower, double upper > using  u16sq12 = sq<uint16_t,  12, lower, upper>;
template< double lower, double upper > using  u16sq13 = sq<uint16_t,  13, lower, upper>;
template< double lower, double upper > using  u16sq14 = sq<uint16_t,  14, lower, upper>;
template< double lower, double upper > using  u16sq15 = sq<uint16_t,  15, lower, upper>;
template< double lower, double upper > using i32sqm16 = sq< int32_t, -16, lower, upper>;
template< double lower, double upper > using i32sqm15 = sq< int32_t, -15, lower, upper>;
template< double lower, double upper > using i32sqm14 = sq< int32_t, -14, lower, upper>;
template< double lower, double upper > using i32sqm13 = sq< int32_t, -13, lower, upper>;
template< double lower, double upper > using i32sqm12 = sq< int32_t, -12, lower, upper>;
template< double lower, double upper > using i32sqm11 = sq< int32_t, -11, lower, upper>;
template< double lower, double upper > using i32sqm10 = sq< int32_t, -10, lower, upper>;
template< double lower, double upper > using  i32sqm9 = sq< int32_t,  -9, lower, upper>;
template< double lower, double upper > using  i32sqm8 = sq< int32_t,  -8, lower, upper>;
template< double lower, double upper > using  i32sqm7 = sq< int32_t,  -7, lower, upper>;
template< double lower, double upper > using  i32sqm6 = sq< int32_t,  -6, lower, upper>;
template< double lower, double upper > using  i32sqm5 = sq< int32_t,  -5, lower, upper>;
template< double lower, double upper > using  i32sqm4 = sq< int32_t,  -4, lower, upper>;
template< double lower, double upper > using  i32sqm3 = sq< int32_t,  -3, lower, upper>;
template< double lower, double upper > using  i32sqm2 = sq< int32_t,  -2, lower, upper>;
template< double lower, double upper > using  i32sqm1 = sq< int32_t,  -1, lower, upper>;
template< double lower, double upper > using   i32sq0 = sq< int32_t,   0, lower, upper>;
template< double lower, double upper > using   i32sq1 = sq< int32_t,   1, lower, upper>;
template< double lower, double upper > using   i32sq2 = sq< int32_t,   2, lower, upper>;
template< double lower, double upper > using   i32sq3 = sq< int32_t,   3, lower, upper>;
template< double lower, double upper > using   i32sq4 = sq< int32_t,   4, lower, upper>;
template< double lower, double upper > using   i32sq5 = sq< int32_t,   5, lower, upper>;
template< double lower, double upper > using   i32sq6 = sq< int32_t,   6, lower, upper>;
template< double lower, double upper > using   i32sq7 = sq< int32_t,   7, lower, upper>;
template< double lower, double upper > using   i32sq8 = sq< int32_t,   8, lower, upper>;
template< double lower, double upper > using   i32sq9 = sq< int32_t,   9, lower, upper>;
template< double lower, double upper > using  i32sq10 = sq< int32_t,  10, lower, upper>;
template< double lower, double upper > using  i32sq11 = sq< int32_t,  11, lower, upper>;
template< double lower, double upper > using  i32sq12 = sq< int32_t,  12, lower, upper>;
template< double lower, double upper > using  i32sq13 = sq< int32_t,  13, lower, upper>;
template< double lower, double upper > using  i32sq14 = sq< int32_t,  14, lower, upper>;
template< double lower, double upper > using  i32sq15 = sq< int32_t,  15, lower, upper>;
template< double lower, double upper > using  i32sq16 = sq< int32_t,  16, lower, upper>;
template< double lower, double upper > using  i32sq17 = sq< int32_t,  17, lower, upper>;
template< double lower, double upper > using  i32sq18 = sq< int32_t,  18, lower, upper>;
template< double lower, double upper > using  i32sq19 = sq< int32_t,  19, lower, upper>;
template< double lower, double upper > using  i32sq20 = sq< int32_t,  20, lower, upper>;
template< double lower, double upper > using  i32sq21 = sq< int32_t,  21, lower, upper>;
template< double lower, double upper > using  i32sq22 = sq< int32_t,  22, lower, upper>;
template< double lower, double upper > using  i32sq23 = sq< int32_t,  23, lower, upper>;
template< double lower, double upper > using  i32sq24 = sq< int32_t,  24, lower, upper>;
template< double lower, double upper > using  i32sq25 = sq< int32_t,  25, lower, upper>;
template< double lower, double upper > using  i32sq26 = sq< int32_t,  26, lower, upper>;
template< double lower, double upper > using  i32sq27 = sq< int32_t,  27, lower, upper>;
template< double lower, double upper > using  i32sq28 = sq< int32_t,  28, lower, upper>;
template< double lower, double upper > using  i32sq29 = sq< int32_t,  29, lower, upper>;
template< double lower, double upper > using  i32sq30 = sq< int32_t,  30, lower, upper>;
template< double lower, double upper > using  i32sq31 = sq< int32_t,  31, lower, upper>;
template< double lower, double upper > using u32sqm16 = sq<uint32_t, -16, lower, upper>;
template< double lower, double upper > using u32sqm15 = sq<uint32_t, -15, lower, upper>;
template< double lower, double upper > using u32sqm14 = sq<uint32_t, -14, lower, upper>;
template< double lower, double upper > using u32sqm13 = sq<uint32_t, -13, lower, upper>;
template< double lower, double upper > using u32sqm12 = sq<uint32_t, -12, lower, upper>;
template< double lower, double upper > using u32sqm11 = sq<uint32_t, -11, lower, upper>;
template< double lower, double upper > using u32sqm10 = sq<uint32_t, -10, lower, upper>;
template< double lower, double upper > using  u32sqm9 = sq<uint32_t,  -9, lower, upper>;
template< double lower, double upper > using  u32sqm8 = sq<uint32_t,  -8, lower, upper>;
template< double lower, double upper > using  u32sqm7 = sq<uint32_t,  -7, lower, upper>;
template< double lower, double upper > using  u32sqm6 = sq<uint32_t,  -6, lower, upper>;
template< double lower, double upper > using  u32sqm5 = sq<uint32_t,  -5, lower, upper>;
template< double lower, double upper > using  u32sqm4 = sq<uint32_t,  -4, lower, upper>;
template< double lower, double upper > using  u32sqm3 = sq<uint32_t,  -3, lower, upper>;
template< double lower, double upper > using  u32sqm2 = sq<uint32_t,  -2, lower, upper>;
template< double lower, double upper > using  u32sqm1 = sq<uint32_t,  -1, lower, upper>;
template< double lower, double upper > using   u32sq0 = sq<uint32_t,   0, lower, upper>;
template< double lower, double upper > using   u32sq1 = sq<uint32_t,   1, lower, upper>;
template< double lower, double upper > using   u32sq2 = sq<uint32_t,   2, lower, upper>;
template< double lower, double upper > using   u32sq3 = sq<uint32_t,   3, lower, upper>;
template< double lower, double upper > using   u32sq4 = sq<uint32_t,   4, lower, upper>;
template< double lower, double upper > using   u32sq5 = sq<uint32_t,   5, lower, upper>;
template< double lower, double upper > using   u32sq6 = sq<uint32_t,   6, lower, upper>;
template< double lower, double upper > using   u32sq7 = sq<uint32_t,   7, lower, upper>;
template< double lower, double upper > using   u32sq8 = sq<uint32_t,   8, lower, upper>;
template< double lower, double upper > using   u32sq9 = sq<uint32_t,   9, lower, upper>;
template< double lower, double upper > using  u32sq10 = sq<uint32_t,  10, lower, upper>;
template< double lower, double upper > using  u32sq11 = sq<uint32_t,  11, lower, upper>;
template< double lower, double upper > using  u32sq12 = sq<uint32_t,  12, lower, upper>;
template< double lower, double upper > using  u32sq13 = sq<uint32_t,  13, lower, upper>;
template< double lower, double upper > using  u32sq14 = sq<uint32_t,  14, lower, upper>;
template< double lower, double upper > using  u32sq15 = sq<uint32_t,  15, lower, upper>;
template< double lower, double upper > using  u32sq16 = sq<uint32_t,  16, lower, upper>;
template< double lower, double upper > using  u32sq17 = sq<uint32_t,  17, lower, upper>;
template< double lower, double upper > using  u32sq18 = sq<uint32_t,  18, lower, upper>;
template< double lower, double upper > using  u32sq19 = sq<uint32_t,  19, lower, upper>;
template< double lower, double upper > using  u32sq20 = sq<uint32_t,  20, lower, upper>;
template< double lower, double upper > using  u32sq21 = sq<uint32_t,  21, lower, upper>;
template< double lower, double upper > using  u32sq22 = sq<uint32_t,  22, lower, upper>;
template< double lower, double upper > using  u32sq23 = sq<uint32_t,  23, lower, upper>;
template< double lower, double upper > using  u32sq24 = sq<uint32_t,  24, lower, upper>;
template< double lower, double upper > using  u32sq25 = sq<uint32_t,  25, lower, upper>;
template< double lower, double upper > using  u32sq26 = sq<uint32_t,  26, lower, upper>;
template< double lower, double upper > using  u32sq27 = sq<uint32_t,  27, lower, upper>;
template< double lower, double upper > using  u32sq28 = sq<uint32_t,  28, lower, upper>;
template< double lower, double upper > using  u32sq29 = sq<uint32_t,  29, lower, upper>;
template< double lower, double upper > using  u32sq30 = sq<uint32_t,  30, lower, upper>;
template< double lower, double upper > using  u32sq31 = sq<uint32_t,  31, lower, upper>;

/* literal operators */

template< char ...chars > consteval auto operator""_i8sqm4()   { return sqFromLiteral<  int8_t,  -4, chars...>(); }
template< char ...chars > consteval auto operator""_i8sqm3()   { return sqFromLiteral<  int8_t,  -3, chars...>(); }
template< char ...chars > consteval auto operator""_i8sqm2()   { return sqFromLiteral<  int8_t,  -2, chars...>(); }
template< char ...chars > consteval auto operator""_i8sqm1()   { return sqFromLiteral<  int8_t,  -1, chars...>(); }
template< char ...chars > consteval auto operator""_i8sq0()    { return sqFromLiteral<  int8_t,   0, chars...>(); }
template< char ...chars > consteval auto operator""_i8sq1()    { return sqFromLiteral<  int8_t,   1, chars...>(); }
template< char ...chars > consteval auto operator""_i8sq2()    { return sqFromLiteral<  int8_t,   2, chars...>(); }
template< char ...chars > consteval auto operator""_i8sq3()    { return sqFromLiteral<  int8_t,   3, chars...>(); }
template< char ...chars > consteval auto operator""_i8sq4()    { return sqFromLiteral<  int8_t,   4, chars...>(); }
template< char ...chars > consteval auto operator""_i8sq5()    { return sqFromLiteral<  int8_t,   5, chars...>(); }
template< char ...chars > consteval auto operator""_i8sq6()    { return sqFromLiteral<  int8_t,   6, chars...>(); }
template< char ...chars > consteval auto operator""_i8sq7()    { return sqFromLiteral<  int8_t,   7, chars...>(); }
template< char ...chars > consteval auto operator""_u8sqm4()   { return sqFromLiteral< uint8_t,  -4, chars...>(); }
template< char ...chars > consteval auto operator""_u8sqm3()   { return sqFromLiteral< uint8_t,  -3, chars...>(); }
template< char ...chars > consteval auto operator""_u8sqm2()   { return sqFromLiteral< uint8_t,  -2, chars...>(); }
template< char ...chars > consteval auto operator""_u8sqm1()   { return sqFromLiteral< uint8_t,  -1, chars...>(); }
template< char ...chars > consteval auto operator""_u8sq0()    { return sqFromLiteral< uint8_t,   0, chars...>(); }
template< char ...chars > consteval auto operator""_u8sq1()    { return sqFromLiteral< uint8_t,   1, chars...>(); }
template< char ...chars > consteval auto operator""_u8sq2()    { return sqFromLiteral< uint8_t,   2, chars...>(); }
template< char ...chars > consteval auto operator""_u8sq3()    { return sqFromLiteral< uint8_t,   3, chars...>(); }
template< char ...chars > consteval auto operator""_u8sq4()    { return sqFromLiteral< uint8_t,   4, chars...>(); }
template< char ...chars > consteval auto operator""_u8sq5()    { return sqFromLiteral< uint8_t,   5, chars...>(); }
template< char ...chars > consteval auto operator""_u8sq6()    { return sqFromLiteral< uint8_t,   6, chars...>(); }
template< char ...chars > consteval auto operator""_u8sq7()    { return sqFromLiteral< uint8_t,   7, chars...>(); }
template< char ...chars > consteval auto operator""_i16sqm8()  { return sqFromLiteral< int16_t,  -8, chars...>(); }
template< char ...chars > consteval auto operator""_i16sqm7()  { return sqFromLiteral< int16_t,  -7, chars...>(); }
template< char ...chars > consteval auto operator""_i16sqm6()  { return sqFromLiteral< int16_t,  -6, chars...>(); }
template< char ...chars > consteval auto operator""_i16sqm5()  { return sqFromLiteral< int16_t,  -5, chars...>(); }
template< char ...chars > consteval auto operator""_i16sqm4()  { return sqFromLiteral< int16_t,  -4, chars...>(); }
template< char ...chars > consteval auto operator""_i16sqm3()  { return sqFromLiteral< int16_t,  -3, chars...>(); }
template< char ...chars > consteval auto operator""_i16sqm2()  { return sqFromLiteral< int16_t,  -2, chars...>(); }
template< char ...chars > consteval auto operator""_i16sqm1()  { return sqFromLiteral< int16_t,  -1, chars...>(); }
template< char ...chars > consteval auto operator""_i16sq0()   { return sqFromLiteral< int16_t,   0, chars...>(); }
template< char ...chars > consteval auto operator""_i16sq1()   { return sqFromLiteral< int16_t,   1, chars...>(); }
template< char ...chars > consteval auto operator""_i16sq2()   { return sqFromLiteral< int16_t,   2, chars...>(); }
template< char ...chars > consteval auto operator""_i16sq3()   { return sqFromLiteral< int16_t,   3, chars...>(); }
template< char ...chars > consteval auto operator""_i16sq4()   { return sqFromLiteral< int16_t,   4, chars...>(); }
template< char ...chars > consteval auto operator""_i16sq5()   { return sqFromLiteral< int16_t,   5, chars...>(); }
template< char ...chars > consteval auto operator""_i16sq6()   { return sqFromLiteral< int16_t,   6, chars...>(); }
template< char ...chars > consteval auto operator""_i16sq7()   { return sqFromLiteral< int16_t,   7, chars...>(); }
template< char ...chars > consteval auto operator""_i16sq8()   { return sqFromLiteral< int16_t,   8, chars...>(); }
template< char ...chars > consteval auto operator""_i16sq9()   { return sqFromLiteral< int16_t,   9, chars...>(); }
template< char ...chars > consteval auto operator""_i16sq10()  { return sqFromLiteral< int16_t,  10, chars...>(); }
template< char ...chars > consteval auto operator""_i16sq11()  { return sqFromLiteral< int16_t,  11, chars...>(); }
template< char ...chars > consteval auto operator""_i16sq12()  { return sqFromLiteral< int16_t,  12, chars...>(); }
template< char ...chars > consteval auto operator""_i16sq13()  { return sqFromLiteral< int16_t,  13, chars...>(); }
template< char ...chars > consteval auto operator""_i16sq14()  { return sqFromLiteral< int16_t,  14, chars...>(); }
template< char ...chars > consteval auto operator""_i16sq15()  { return sqFromLiteral< int16_t,  15, chars...>(); }
template< char ...chars > consteval auto operator""_u16sqm8()  { return sqFromLiteral<uint16_t,  -8, chars...>(); }
template< char ...chars > consteval auto operator""_u16sqm7()  { return sqFromLiteral<uint16_t,  -7, chars...>(); }
template< char ...chars > consteval auto operator""_u16sqm6()  { return sqFromLiteral<uint16_t,  -6, chars...>(); }
template< char ...chars > consteval auto operator""_u16sqm5()  { return sqFromLiteral<uint16_t,  -5, chars...>(); }
template< char ...chars > consteval auto operator""_u16sqm4()  { return sqFromLiteral<uint16_t,  -4, chars...>(); }
template< char ...chars > consteval auto operator""_u16sqm3()  { return sqFromLiteral<uint16_t,  -3, chars...>(); }
template< char ...chars > consteval auto operator""_u16sqm2()  { return sqFromLiteral<uint16_t,  -2, chars...>(); }
template< char ...chars > consteval auto operator""_u16sqm1()  { return sqFromLiteral<uint16_t,  -1, chars...>(); }
template< char ...chars > consteval auto operator""_u16sq0()   { return sqFromLiteral<uint16_t,   0, chars...>(); }
template< char ...chars > consteval auto operator""_u16sq1()   { return sqFromLiteral<uint16_t,   1, chars...>(); }
template< char ...chars > consteval auto operator""_u16sq2()   { return sqFromLiteral<uint16_t,   2, chars...>(); }
template< char ...chars > consteval auto operator""_u16sq3()   { return sqFromLiteral<uint16_t,   3, chars...>(); }
template< char ...chars > consteval auto operator""_u16sq4()   { return sqFromLiteral<uint16_t,   4, chars...>(); }
template< char ...chars > consteval auto operator""_u16sq5()   { return sqFromLiteral<uint16_t,   5, chars...>(); }
template< char ...chars > consteval auto operator""_u16sq6()   { return sqFromLiteral<uint16_t,   6, chars...>(); }
template< char ...chars > consteval auto operator""_u16sq7()   { return sqFromLiteral<uint16_t,   7, chars...>(); }
template< char ...chars > consteval auto operator""_u16sq8()   { return sqFromLiteral<uint16_t,   8, chars...>(); }
template< char ...chars > consteval auto operator""_u16sq9()   { return sqFromLiteral<uint16_t,   9, chars...>(); }
template< char ...chars > consteval auto operator""_u16sq10()  { return sqFromLiteral<uint16_t,  10, chars...>(); }
template< char ...chars > consteval auto operator""_u16sq11()  { return sqFromLiteral<uint16_t,  11, chars...>(); }
template< char ...chars > consteval auto operator""_u16sq12()  { return sqFromLiteral<uint16_t,  12, chars...>(); }
template< char ...chars > consteval auto operator""_u16sq13()  { return sqFromLiteral<uint16_t,  13, chars...>(); }
template< char ...chars > consteval auto operator""_u16sq14()  { return sqFromLiteral<uint16_t,  14, chars...>(); }
template< char ...chars > consteval auto operator""_u16sq15()  { return sqFromLiteral<uint16_t,  15, chars...>(); }
template< char ...chars > consteval auto operator""_i32sqm16() { return sqFromLiteral< int32_t, -16, chars...>(); }
template< char ...chars > consteval auto operator""_i32sqm15() { return sqFromLiteral< int32_t, -15, chars...>(); }
template< char ...chars > consteval auto operator""_i32sqm14() { return sqFromLiteral< int32_t, -14, chars...>(); }
template< char ...chars > consteval auto operator""_i32sqm13() { return sqFromLiteral< int32_t, -13, chars...>(); }
template< char ...chars > consteval auto operator""_i32sqm12() { return sqFromLiteral< int32_t, -12, chars...>(); }
template< char ...chars > consteval auto operator""_i32sqm11() { return sqFromLiteral< int32_t, -11, chars...>(); }
template< char ...chars > consteval auto operator""_i32sqm10() { return sqFromLiteral< int32_t, -10, chars...>(); }
template< char ...chars > consteval auto operator""_i32sqm9()  { return sqFromLiteral< int32_t,  -9, chars...>(); }
template< char ...chars > consteval auto operator""_i32sqm8()  { return sqFromLiteral< int32_t,  -8, chars...>(); }
template< char ...chars > consteval auto operator""_i32sqm7()  { return sqFromLiteral< int32_t,  -7, chars...>(); }
template< char ...chars > consteval auto operator""_i32sqm6()  { return sqFromLiteral< int32_t,  -6, chars...>(); }
template< char ...chars > consteval auto operator""_i32sqm5()  { return sqFromLiteral< int32_t,  -5, chars...>(); }
template< char ...chars > consteval auto operator""_i32sqm4()  { return sqFromLiteral< int32_t,  -4, chars...>(); }
template< char ...chars > consteval auto operator""_i32sqm3()  { return sqFromLiteral< int32_t,  -3, chars...>(); }
template< char ...chars > consteval auto operator""_i32sqm2()  { return sqFromLiteral< int32_t,  -2, chars...>(); }
template< char ...chars > consteval auto operator""_i32sqm1()  { return sqFromLiteral< int32_t,  -1, chars...>(); }
template< char ...chars > consteval auto operator""_i32sq0()   { return sqFromLiteral< int32_t,   0, chars...>(); }
template< char ...chars > consteval auto operator""_i32sq1()   { return sqFromLiteral< int32_t,   1, chars...>(); }
template< char ...chars > consteval auto operator""_i32sq2()   { return sqFromLiteral< int32_t,   2, chars...>(); }
template< char ...chars > consteval auto operator""_i32sq3()   { return sqFromLiteral< int32_t,   3, chars...>(); }
template< char ...chars > consteval auto operator""_i32sq4()   { return sqFromLiteral< int32_t,   4, chars...>(); }
template< char ...chars > consteval auto operator""_i32sq5()   { return sqFromLiteral< int32_t,   5, chars...>(); }
template< char ...chars > consteval auto operator""_i32sq6()   { return sqFromLiteral< int32_t,   6, chars...>(); }
template< char ...chars > consteval auto operator""_i32sq7()   { return sqFromLiteral< int32_t,   7, chars...>(); }
template< char ...chars > consteval auto operator""_i32sq8()   { return sqFromLiteral< int32_t,   8, chars...>(); }
template< char ...chars > consteval auto operator""_i32sq9()   { return sqFromLiteral< int32_t,   9, chars...>(); }
template< char ...chars > consteval auto operator""_i32sq10()  { return sqFromLiteral< int32_t,  10, chars...>(); }
template< char ...chars > consteval auto operator""_i32sq11()  { return sqFromLiteral< int32_t,  11, chars...>(); }
template< char ...chars > consteval auto operator""_i32sq12()  { return sqFromLiteral< int32_t,  12, chars...>(); }
template< char ...chars > consteval auto operator""_i32sq13()  { return sqFromLiteral< int32_t,  13, chars...>(); }
template< char ...chars > consteval auto operator""_i32sq14()  { return sqFromLiteral< int32_t,  14, chars...>(); }
template< char ...chars > consteval auto operator""_i32sq15()  { return sqFromLiteral< int32_t,  15, chars...>(); }
template< char ...chars > consteval auto operator""_i32sq16()  { return sqFromLiteral< int32_t,  16, chars...>(); }
template< char ...chars > consteval auto operator""_i32sq17()  { return sqFromLiteral< int32_t,  17, chars...>(); }
template< char ...chars > consteval auto operator""_i32sq18()  { return sqFromLiteral< int32_t,  18, chars...>(); }
template< char ...chars > consteval auto operator""_i32sq19()  { return sqFromLiteral< int32_t,  19, chars...>(); }
template< char ...chars > consteval auto operator""_i32sq20()  { return sqFromLiteral< int32_t,  20, chars...>(); }
template< char ...chars > consteval auto operator""_i32sq21()  { return sqFromLiteral< int32_t,  21, chars...>(); }
template< char ...chars > consteval auto operator""_i32sq22()  { return sqFromLiteral< int32_t,  22, chars...>(); }
template< char ...chars > consteval auto operator""_i32sq23()  { return sqFromLiteral< int32_t,  23, chars...>(); }
template< char ...chars > consteval auto operator""_i32sq24()  { return sqFromLiteral< int32_t,  24, chars...>(); }
template< char ...chars > consteval auto operator""_i32sq25()  { return sqFromLiteral< int32_t,  25, chars...>(); }
template< char ...chars > consteval auto operator""_i32sq26()  { return sqFromLiteral< int32_t,  26, chars...>(); }
template< char ...chars > consteval auto operator""_i32sq27()  { return sqFromLiteral< int32_t,  27, chars...>(); }
template< char ...chars > consteval auto operator""_i32sq28()  { return sqFromLiteral< int32_t,  28, chars...>(); }
template< char ...chars > consteval auto operator""_i32sq29()  { return sqFromLiteral< int32_t,  29, chars...>(); }
template< char ...chars > consteval auto operator""_i32sq30()  { return sqFromLiteral< int32_t,  30, chars...>(); }
template< char ...chars > consteval auto operator""_i32sq31()  { return sqFromLiteral< int32_t,  31, chars...>(); }
template< char ...chars > consteval auto operator""_u32sqm16() { return sqFromLiteral<uint32_t, -16, chars...>(); }
template< char ...chars > consteval auto operator""_u32sqm15() { return sqFromLiteral<uint32_t, -15, chars...>(); }
template< char ...chars > consteval auto operator""_u32sqm14() { return sqFromLiteral<uint32_t, -14, chars...>(); }
template< char ...chars > consteval auto operator""_u32sqm13() { return sqFromLiteral<uint32_t, -13, chars...>(); }
template< char ...chars > consteval auto operator""_u32sqm12() { return sqFromLiteral<uint32_t, -12, chars...>(); }
template< char ...chars > consteval auto operator""_u32sqm11() { return sqFromLiteral<uint32_t, -11, chars...>(); }
template< char ...chars > consteval auto operator""_u32sqm10() { return sqFromLiteral<uint32_t, -10, chars...>(); }
template< char ...chars > consteval auto operator""_u32sqm9()  { return sqFromLiteral<uint32_t,  -9, chars...>(); }
template< char ...chars > consteval auto operator""_u32sqm8()  { return sqFromLiteral<uint32_t,  -8, chars...>(); }
template< char ...chars > consteval auto operator""_u32sqm7()  { return sqFromLiteral<uint32_t,  -7, chars...>(); }
template< char ...chars > consteval auto operator""_u32sqm6()  { return sqFromLiteral<uint32_t,  -6, chars...>(); }
template< char ...chars > consteval auto operator""_u32sqm5()  { return sqFromLiteral<uint32_t,  -5, chars...>(); }
template< char ...chars > consteval auto operator""_u32sqm4()  { return sqFromLiteral<uint32_t,  -4, chars...>(); }
template< char ...chars > consteval auto operator""_u32sqm3()  { return sqFromLiteral<uint32_t,  -3, chars...>(); }
template< char ...chars > consteval auto operator""_u32sqm2()  { return sqFromLiteral<uint32_t,  -2, chars...>(); }
template< char ...chars > consteval auto operator""_u32sqm1()  { return sqFromLiteral<uint32_t,  -1, chars...>(); }
template< char ...chars > consteval auto operator""_u32sq0()   { return sqFromLiteral<uint32_t,   0, chars...>(); }
template< char ...chars > consteval auto operator""_u32sq1()   { return sqFromLiteral<uint32_t,   1, chars...>(); }
template< char ...chars > consteval auto operator""_u32sq2()   { return sqFromLiteral<uint32_t,   2, chars...>(); }
template< char ...chars > consteval auto operator""_u32sq3()   { return sqFromLiteral<uint32_t,   3, chars...>(); }
template< char ...chars > consteval auto operator""_u32sq4()   { return sqFromLiteral<uint32_t,   4, chars...>(); }
template< char ...chars > consteval auto operator""_u32sq5()   { return sqFromLiteral<uint32_t,   5, chars...>(); }
template< char ...chars > consteval auto operator""_u32sq6()   { return sqFromLiteral<uint32_t,   6, chars...>(); }
template< char ...chars > consteval auto operator""_u32sq7()   { return sqFromLiteral<uint32_t,   7, chars...>(); }
template< char ...chars > consteval auto operator""_u32sq8()   { return sqFromLiteral<uint32_t,   8, chars...>(); }
template< char ...chars > consteval auto operator""_u32sq9()   { return sqFromLiteral<uint32_t,   9, chars...>(); }
template< char ...chars > consteval auto operator""_u32sq10()  { return sqFromLiteral<uint32_t,  10, chars...>(); }
template< char ...chars > consteval auto operator""_u32sq11()  { return sqFromLiteral<uint32_t,  11, chars...>(); }
template< char ...chars > consteval auto operator""_u32sq12()  { return sqFromLiteral<uint32_t,  12, chars...>(); }
template< char ...chars > consteval auto operator""_u32sq13()  { return sqFromLiteral<uint32_t,  13, chars...>(); }
template< char ...chars > consteval auto operator""_u32sq14()  { return sqFromLiteral<uint32_t,  14, chars...>(); }
template< char ...chars > consteval auto operator""_u32sq15()  { return sqFromLiteral<uint32_t,  15, chars...>(); }
template< char ...chars > consteval auto operator""_u32sq16()  { return sqFromLiteral<uint32_t,  16, chars...>(); }
template< char ...chars > consteval auto operator""_u32sq17()  { return sqFromLiteral<uint32_t,  17, chars...>(); }
template< char ...chars > consteval auto operator""_u32sq18()  { return sqFromLiteral<uint32_t,  18, chars...>(); }
template< char ...chars > consteval auto operator""_u32sq19()  { return sqFromLiteral<uint32_t,  19, chars...>(); }
template< char ...chars > consteval auto operator""_u32sq20()  { return sqFromLiteral<uint32_t,  20, chars...>(); }
template< char ...chars > consteval auto operator""_u32sq21()  { return sqFromLiteral<uint32_t,  21, chars...>(); }
template< char ...chars > consteval auto operator""_u32sq22()  { return sqFromLiteral<uint32_t,  22, chars...>(); }
template< char ...chars > consteval auto operator""_u32sq23()  { return sqFromLiteral<uint32_t,  23, chars...>(); }
template< char ...chars > consteval auto operator""_u32sq24()  { return sqFromLiteral<uint32_t,  24, chars...>(); }
template< char ...chars > consteval auto operator""_u32sq25()  { return sqFromLiteral<uint32_t,  25, chars...>(); }
template< char ...chars > consteval auto operator""_u32sq26()  { return sqFromLiteral<uint32_t,  26, chars...>(); }
template< char ...chars > consteval auto operator""_u32sq27()  { return sqFromLiteral<uint32_t,  27, chars...>(); }
template< char ...chars > consteval auto operator""_u32sq28()  { return sqFromLiteral<uint32_t,  28, chars...>(); }
template< char ...chars > consteval auto operator""_u32sq29()  { return sqFromLiteral<uint32_t,  29, chars...>(); }
template< char ...chars > consteval auto operator""_u32sq30()  { return sqFromLiteral<uint32_t,  30, chars...>(); }
template< char ...chars > consteval auto operator""_u32sq31()  { return sqFromLiteral<uint32_t,  31, chars...>(); }

}

#endif
// EOF
