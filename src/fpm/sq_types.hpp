/** \file
 * Declaration of the predefined sq types and literals.
 */

#ifndef FPM_FPM_SQ_TYPES_HPP_
#define FPM_FPM_SQ_TYPES_HPP_

#include "sq.hpp"

namespace fpm::sq::inline types {
/** \addtogroup grp_fpmSq
 * \{ */

/* sq types */

template< double lower = details::lowestRealVMin<  int8_t,  -4>(), double upper = details::highestRealVMax<  int8_t,  -4>() > using   i8sqm4 = Sq<  int8_t,  -4, lower, upper>;
template< double lower = details::lowestRealVMin<  int8_t,  -3>(), double upper = details::highestRealVMax<  int8_t,  -3>() > using   i8sqm3 = Sq<  int8_t,  -3, lower, upper>;
template< double lower = details::lowestRealVMin<  int8_t,  -2>(), double upper = details::highestRealVMax<  int8_t,  -2>() > using   i8sqm2 = Sq<  int8_t,  -2, lower, upper>;
template< double lower = details::lowestRealVMin<  int8_t,  -1>(), double upper = details::highestRealVMax<  int8_t,  -1>() > using   i8sqm1 = Sq<  int8_t,  -1, lower, upper>;
template< double lower = details::lowestRealVMin<  int8_t,   0>(), double upper = details::highestRealVMax<  int8_t,   0>() > using    i8sq0 = Sq<  int8_t,   0, lower, upper>;
template< double lower = details::lowestRealVMin<  int8_t,   1>(), double upper = details::highestRealVMax<  int8_t,   1>() > using    i8sq1 = Sq<  int8_t,   1, lower, upper>;
template< double lower = details::lowestRealVMin<  int8_t,   2>(), double upper = details::highestRealVMax<  int8_t,   2>() > using    i8sq2 = Sq<  int8_t,   2, lower, upper>;
template< double lower = details::lowestRealVMin<  int8_t,   3>(), double upper = details::highestRealVMax<  int8_t,   3>() > using    i8sq3 = Sq<  int8_t,   3, lower, upper>;
template< double lower = details::lowestRealVMin<  int8_t,   4>(), double upper = details::highestRealVMax<  int8_t,   4>() > using    i8sq4 = Sq<  int8_t,   4, lower, upper>;
template< double lower = details::lowestRealVMin<  int8_t,   5>(), double upper = details::highestRealVMax<  int8_t,   5>() > using    i8sq5 = Sq<  int8_t,   5, lower, upper>;
template< double lower = details::lowestRealVMin<  int8_t,   6>(), double upper = details::highestRealVMax<  int8_t,   6>() > using    i8sq6 = Sq<  int8_t,   6, lower, upper>;
template< double lower = details::lowestRealVMin<  int8_t,   7>(), double upper = details::highestRealVMax<  int8_t,   7>() > using    i8sq7 = Sq<  int8_t,   7, lower, upper>;
template< double lower = details::lowestRealVMin< uint8_t,  -4>(), double upper = details::highestRealVMax< uint8_t,  -4>() > using   u8sqm4 = Sq< uint8_t,  -4, lower, upper>;
template< double lower = details::lowestRealVMin< uint8_t,  -3>(), double upper = details::highestRealVMax< uint8_t,  -3>() > using   u8sqm3 = Sq< uint8_t,  -3, lower, upper>;
template< double lower = details::lowestRealVMin< uint8_t,  -2>(), double upper = details::highestRealVMax< uint8_t,  -2>() > using   u8sqm2 = Sq< uint8_t,  -2, lower, upper>;
template< double lower = details::lowestRealVMin< uint8_t,  -1>(), double upper = details::highestRealVMax< uint8_t,  -1>() > using   u8sqm1 = Sq< uint8_t,  -1, lower, upper>;
template< double lower = details::lowestRealVMin< uint8_t,   0>(), double upper = details::highestRealVMax< uint8_t,   0>() > using    u8sq0 = Sq< uint8_t,   0, lower, upper>;
template< double lower = details::lowestRealVMin< uint8_t,   1>(), double upper = details::highestRealVMax< uint8_t,   1>() > using    u8sq1 = Sq< uint8_t,   1, lower, upper>;
template< double lower = details::lowestRealVMin< uint8_t,   2>(), double upper = details::highestRealVMax< uint8_t,   2>() > using    u8sq2 = Sq< uint8_t,   2, lower, upper>;
template< double lower = details::lowestRealVMin< uint8_t,   3>(), double upper = details::highestRealVMax< uint8_t,   3>() > using    u8sq3 = Sq< uint8_t,   3, lower, upper>;
template< double lower = details::lowestRealVMin< uint8_t,   4>(), double upper = details::highestRealVMax< uint8_t,   4>() > using    u8sq4 = Sq< uint8_t,   4, lower, upper>;
template< double lower = details::lowestRealVMin< uint8_t,   5>(), double upper = details::highestRealVMax< uint8_t,   5>() > using    u8sq5 = Sq< uint8_t,   5, lower, upper>;
template< double lower = details::lowestRealVMin< uint8_t,   6>(), double upper = details::highestRealVMax< uint8_t,   6>() > using    u8sq6 = Sq< uint8_t,   6, lower, upper>;
template< double lower = details::lowestRealVMin< uint8_t,   7>(), double upper = details::highestRealVMax< uint8_t,   7>() > using    u8sq7 = Sq< uint8_t,   7, lower, upper>;
template< double lower = details::lowestRealVMin< int16_t,  -8>(), double upper = details::highestRealVMax< int16_t,  -8>() > using  i16sqm8 = Sq< int16_t,  -8, lower, upper>;
template< double lower = details::lowestRealVMin< int16_t,  -7>(), double upper = details::highestRealVMax< int16_t,  -7>() > using  i16sqm7 = Sq< int16_t,  -7, lower, upper>;
template< double lower = details::lowestRealVMin< int16_t,  -6>(), double upper = details::highestRealVMax< int16_t,  -6>() > using  i16sqm6 = Sq< int16_t,  -6, lower, upper>;
template< double lower = details::lowestRealVMin< int16_t,  -5>(), double upper = details::highestRealVMax< int16_t,  -5>() > using  i16sqm5 = Sq< int16_t,  -5, lower, upper>;
template< double lower = details::lowestRealVMin< int16_t,  -4>(), double upper = details::highestRealVMax< int16_t,  -4>() > using  i16sqm4 = Sq< int16_t,  -4, lower, upper>;
template< double lower = details::lowestRealVMin< int16_t,  -3>(), double upper = details::highestRealVMax< int16_t,  -3>() > using  i16sqm3 = Sq< int16_t,  -3, lower, upper>;
template< double lower = details::lowestRealVMin< int16_t,  -2>(), double upper = details::highestRealVMax< int16_t,  -2>() > using  i16sqm2 = Sq< int16_t,  -2, lower, upper>;
template< double lower = details::lowestRealVMin< int16_t,  -1>(), double upper = details::highestRealVMax< int16_t,  -1>() > using  i16sqm1 = Sq< int16_t,  -1, lower, upper>;
template< double lower = details::lowestRealVMin< int16_t,   0>(), double upper = details::highestRealVMax< int16_t,   0>() > using   i16sq0 = Sq< int16_t,   0, lower, upper>;
template< double lower = details::lowestRealVMin< int16_t,   1>(), double upper = details::highestRealVMax< int16_t,   1>() > using   i16sq1 = Sq< int16_t,   1, lower, upper>;
template< double lower = details::lowestRealVMin< int16_t,   2>(), double upper = details::highestRealVMax< int16_t,   2>() > using   i16sq2 = Sq< int16_t,   2, lower, upper>;
template< double lower = details::lowestRealVMin< int16_t,   3>(), double upper = details::highestRealVMax< int16_t,   3>() > using   i16sq3 = Sq< int16_t,   3, lower, upper>;
template< double lower = details::lowestRealVMin< int16_t,   4>(), double upper = details::highestRealVMax< int16_t,   4>() > using   i16sq4 = Sq< int16_t,   4, lower, upper>;
template< double lower = details::lowestRealVMin< int16_t,   5>(), double upper = details::highestRealVMax< int16_t,   5>() > using   i16sq5 = Sq< int16_t,   5, lower, upper>;
template< double lower = details::lowestRealVMin< int16_t,   6>(), double upper = details::highestRealVMax< int16_t,   6>() > using   i16sq6 = Sq< int16_t,   6, lower, upper>;
template< double lower = details::lowestRealVMin< int16_t,   7>(), double upper = details::highestRealVMax< int16_t,   7>() > using   i16sq7 = Sq< int16_t,   7, lower, upper>;
template< double lower = details::lowestRealVMin< int16_t,   8>(), double upper = details::highestRealVMax< int16_t,   8>() > using   i16sq8 = Sq< int16_t,   8, lower, upper>;
template< double lower = details::lowestRealVMin< int16_t,   9>(), double upper = details::highestRealVMax< int16_t,   9>() > using   i16sq9 = Sq< int16_t,   9, lower, upper>;
template< double lower = details::lowestRealVMin< int16_t,  10>(), double upper = details::highestRealVMax< int16_t,  10>() > using  i16sq10 = Sq< int16_t,  10, lower, upper>;
template< double lower = details::lowestRealVMin< int16_t,  11>(), double upper = details::highestRealVMax< int16_t,  11>() > using  i16sq11 = Sq< int16_t,  11, lower, upper>;
template< double lower = details::lowestRealVMin< int16_t,  12>(), double upper = details::highestRealVMax< int16_t,  12>() > using  i16sq12 = Sq< int16_t,  12, lower, upper>;
template< double lower = details::lowestRealVMin< int16_t,  13>(), double upper = details::highestRealVMax< int16_t,  13>() > using  i16sq13 = Sq< int16_t,  13, lower, upper>;
template< double lower = details::lowestRealVMin< int16_t,  14>(), double upper = details::highestRealVMax< int16_t,  14>() > using  i16sq14 = Sq< int16_t,  14, lower, upper>;
template< double lower = details::lowestRealVMin< int16_t,  15>(), double upper = details::highestRealVMax< int16_t,  15>() > using  i16sq15 = Sq< int16_t,  15, lower, upper>;
template< double lower = details::lowestRealVMin<uint16_t,  -8>(), double upper = details::highestRealVMax<uint16_t,  -8>() > using  u16sqm8 = Sq<uint16_t,  -8, lower, upper>;
template< double lower = details::lowestRealVMin<uint16_t,  -7>(), double upper = details::highestRealVMax<uint16_t,  -7>() > using  u16sqm7 = Sq<uint16_t,  -7, lower, upper>;
template< double lower = details::lowestRealVMin<uint16_t,  -6>(), double upper = details::highestRealVMax<uint16_t,  -6>() > using  u16sqm6 = Sq<uint16_t,  -6, lower, upper>;
template< double lower = details::lowestRealVMin<uint16_t,  -5>(), double upper = details::highestRealVMax<uint16_t,  -5>() > using  u16sqm5 = Sq<uint16_t,  -5, lower, upper>;
template< double lower = details::lowestRealVMin<uint16_t,  -4>(), double upper = details::highestRealVMax<uint16_t,  -4>() > using  u16sqm4 = Sq<uint16_t,  -4, lower, upper>;
template< double lower = details::lowestRealVMin<uint16_t,  -3>(), double upper = details::highestRealVMax<uint16_t,  -3>() > using  u16sqm3 = Sq<uint16_t,  -3, lower, upper>;
template< double lower = details::lowestRealVMin<uint16_t,  -2>(), double upper = details::highestRealVMax<uint16_t,  -2>() > using  u16sqm2 = Sq<uint16_t,  -2, lower, upper>;
template< double lower = details::lowestRealVMin<uint16_t,  -1>(), double upper = details::highestRealVMax<uint16_t,  -1>() > using  u16sqm1 = Sq<uint16_t,  -1, lower, upper>;
template< double lower = details::lowestRealVMin<uint16_t,   0>(), double upper = details::highestRealVMax<uint16_t,   0>() > using   u16sq0 = Sq<uint16_t,   0, lower, upper>;
template< double lower = details::lowestRealVMin<uint16_t,   1>(), double upper = details::highestRealVMax<uint16_t,   1>() > using   u16sq1 = Sq<uint16_t,   1, lower, upper>;
template< double lower = details::lowestRealVMin<uint16_t,   2>(), double upper = details::highestRealVMax<uint16_t,   2>() > using   u16sq2 = Sq<uint16_t,   2, lower, upper>;
template< double lower = details::lowestRealVMin<uint16_t,   3>(), double upper = details::highestRealVMax<uint16_t,   3>() > using   u16sq3 = Sq<uint16_t,   3, lower, upper>;
template< double lower = details::lowestRealVMin<uint16_t,   4>(), double upper = details::highestRealVMax<uint16_t,   4>() > using   u16sq4 = Sq<uint16_t,   4, lower, upper>;
template< double lower = details::lowestRealVMin<uint16_t,   5>(), double upper = details::highestRealVMax<uint16_t,   5>() > using   u16sq5 = Sq<uint16_t,   5, lower, upper>;
template< double lower = details::lowestRealVMin<uint16_t,   6>(), double upper = details::highestRealVMax<uint16_t,   6>() > using   u16sq6 = Sq<uint16_t,   6, lower, upper>;
template< double lower = details::lowestRealVMin<uint16_t,   7>(), double upper = details::highestRealVMax<uint16_t,   7>() > using   u16sq7 = Sq<uint16_t,   7, lower, upper>;
template< double lower = details::lowestRealVMin<uint16_t,   8>(), double upper = details::highestRealVMax<uint16_t,   8>() > using   u16sq8 = Sq<uint16_t,   8, lower, upper>;
template< double lower = details::lowestRealVMin<uint16_t,   9>(), double upper = details::highestRealVMax<uint16_t,   9>() > using   u16sq9 = Sq<uint16_t,   9, lower, upper>;
template< double lower = details::lowestRealVMin<uint16_t,  10>(), double upper = details::highestRealVMax<uint16_t,  10>() > using  u16sq10 = Sq<uint16_t,  10, lower, upper>;
template< double lower = details::lowestRealVMin<uint16_t,  11>(), double upper = details::highestRealVMax<uint16_t,  11>() > using  u16sq11 = Sq<uint16_t,  11, lower, upper>;
template< double lower = details::lowestRealVMin<uint16_t,  12>(), double upper = details::highestRealVMax<uint16_t,  12>() > using  u16sq12 = Sq<uint16_t,  12, lower, upper>;
template< double lower = details::lowestRealVMin<uint16_t,  13>(), double upper = details::highestRealVMax<uint16_t,  13>() > using  u16sq13 = Sq<uint16_t,  13, lower, upper>;
template< double lower = details::lowestRealVMin<uint16_t,  14>(), double upper = details::highestRealVMax<uint16_t,  14>() > using  u16sq14 = Sq<uint16_t,  14, lower, upper>;
template< double lower = details::lowestRealVMin<uint16_t,  15>(), double upper = details::highestRealVMax<uint16_t,  15>() > using  u16sq15 = Sq<uint16_t,  15, lower, upper>;
template< double lower = details::lowestRealVMin< int32_t, -16>(), double upper = details::highestRealVMax< int32_t, -16>() > using i32sqm16 = Sq< int32_t, -16, lower, upper>;
template< double lower = details::lowestRealVMin< int32_t, -15>(), double upper = details::highestRealVMax< int32_t, -15>() > using i32sqm15 = Sq< int32_t, -15, lower, upper>;
template< double lower = details::lowestRealVMin< int32_t, -14>(), double upper = details::highestRealVMax< int32_t, -14>() > using i32sqm14 = Sq< int32_t, -14, lower, upper>;
template< double lower = details::lowestRealVMin< int32_t, -13>(), double upper = details::highestRealVMax< int32_t, -13>() > using i32sqm13 = Sq< int32_t, -13, lower, upper>;
template< double lower = details::lowestRealVMin< int32_t, -12>(), double upper = details::highestRealVMax< int32_t, -12>() > using i32sqm12 = Sq< int32_t, -12, lower, upper>;
template< double lower = details::lowestRealVMin< int32_t, -11>(), double upper = details::highestRealVMax< int32_t, -11>() > using i32sqm11 = Sq< int32_t, -11, lower, upper>;
template< double lower = details::lowestRealVMin< int32_t, -10>(), double upper = details::highestRealVMax< int32_t, -10>() > using i32sqm10 = Sq< int32_t, -10, lower, upper>;
template< double lower = details::lowestRealVMin< int32_t,  -9>(), double upper = details::highestRealVMax< int32_t,  -9>() > using  i32sqm9 = Sq< int32_t,  -9, lower, upper>;
template< double lower = details::lowestRealVMin< int32_t,  -8>(), double upper = details::highestRealVMax< int32_t,  -8>() > using  i32sqm8 = Sq< int32_t,  -8, lower, upper>;
template< double lower = details::lowestRealVMin< int32_t,  -7>(), double upper = details::highestRealVMax< int32_t,  -7>() > using  i32sqm7 = Sq< int32_t,  -7, lower, upper>;
template< double lower = details::lowestRealVMin< int32_t,  -6>(), double upper = details::highestRealVMax< int32_t,  -6>() > using  i32sqm6 = Sq< int32_t,  -6, lower, upper>;
template< double lower = details::lowestRealVMin< int32_t,  -5>(), double upper = details::highestRealVMax< int32_t,  -5>() > using  i32sqm5 = Sq< int32_t,  -5, lower, upper>;
template< double lower = details::lowestRealVMin< int32_t,  -4>(), double upper = details::highestRealVMax< int32_t,  -4>() > using  i32sqm4 = Sq< int32_t,  -4, lower, upper>;
template< double lower = details::lowestRealVMin< int32_t,  -3>(), double upper = details::highestRealVMax< int32_t,  -3>() > using  i32sqm3 = Sq< int32_t,  -3, lower, upper>;
template< double lower = details::lowestRealVMin< int32_t,  -2>(), double upper = details::highestRealVMax< int32_t,  -2>() > using  i32sqm2 = Sq< int32_t,  -2, lower, upper>;
template< double lower = details::lowestRealVMin< int32_t,  -1>(), double upper = details::highestRealVMax< int32_t,  -1>() > using  i32sqm1 = Sq< int32_t,  -1, lower, upper>;
template< double lower = details::lowestRealVMin< int32_t,   0>(), double upper = details::highestRealVMax< int32_t,   0>() > using   i32sq0 = Sq< int32_t,   0, lower, upper>;
template< double lower = details::lowestRealVMin< int32_t,   1>(), double upper = details::highestRealVMax< int32_t,   1>() > using   i32sq1 = Sq< int32_t,   1, lower, upper>;
template< double lower = details::lowestRealVMin< int32_t,   2>(), double upper = details::highestRealVMax< int32_t,   2>() > using   i32sq2 = Sq< int32_t,   2, lower, upper>;
template< double lower = details::lowestRealVMin< int32_t,   3>(), double upper = details::highestRealVMax< int32_t,   3>() > using   i32sq3 = Sq< int32_t,   3, lower, upper>;
template< double lower = details::lowestRealVMin< int32_t,   4>(), double upper = details::highestRealVMax< int32_t,   4>() > using   i32sq4 = Sq< int32_t,   4, lower, upper>;
template< double lower = details::lowestRealVMin< int32_t,   5>(), double upper = details::highestRealVMax< int32_t,   5>() > using   i32sq5 = Sq< int32_t,   5, lower, upper>;
template< double lower = details::lowestRealVMin< int32_t,   6>(), double upper = details::highestRealVMax< int32_t,   6>() > using   i32sq6 = Sq< int32_t,   6, lower, upper>;
template< double lower = details::lowestRealVMin< int32_t,   7>(), double upper = details::highestRealVMax< int32_t,   7>() > using   i32sq7 = Sq< int32_t,   7, lower, upper>;
template< double lower = details::lowestRealVMin< int32_t,   8>(), double upper = details::highestRealVMax< int32_t,   8>() > using   i32sq8 = Sq< int32_t,   8, lower, upper>;
template< double lower = details::lowestRealVMin< int32_t,   9>(), double upper = details::highestRealVMax< int32_t,   9>() > using   i32sq9 = Sq< int32_t,   9, lower, upper>;
template< double lower = details::lowestRealVMin< int32_t,  10>(), double upper = details::highestRealVMax< int32_t,  10>() > using  i32sq10 = Sq< int32_t,  10, lower, upper>;
template< double lower = details::lowestRealVMin< int32_t,  11>(), double upper = details::highestRealVMax< int32_t,  11>() > using  i32sq11 = Sq< int32_t,  11, lower, upper>;
template< double lower = details::lowestRealVMin< int32_t,  12>(), double upper = details::highestRealVMax< int32_t,  12>() > using  i32sq12 = Sq< int32_t,  12, lower, upper>;
template< double lower = details::lowestRealVMin< int32_t,  13>(), double upper = details::highestRealVMax< int32_t,  13>() > using  i32sq13 = Sq< int32_t,  13, lower, upper>;
template< double lower = details::lowestRealVMin< int32_t,  14>(), double upper = details::highestRealVMax< int32_t,  14>() > using  i32sq14 = Sq< int32_t,  14, lower, upper>;
template< double lower = details::lowestRealVMin< int32_t,  15>(), double upper = details::highestRealVMax< int32_t,  15>() > using  i32sq15 = Sq< int32_t,  15, lower, upper>;
template< double lower = details::lowestRealVMin< int32_t,  16>(), double upper = details::highestRealVMax< int32_t,  16>() > using  i32sq16 = Sq< int32_t,  16, lower, upper>;
template< double lower = details::lowestRealVMin< int32_t,  17>(), double upper = details::highestRealVMax< int32_t,  17>() > using  i32sq17 = Sq< int32_t,  17, lower, upper>;
template< double lower = details::lowestRealVMin< int32_t,  18>(), double upper = details::highestRealVMax< int32_t,  18>() > using  i32sq18 = Sq< int32_t,  18, lower, upper>;
template< double lower = details::lowestRealVMin< int32_t,  19>(), double upper = details::highestRealVMax< int32_t,  19>() > using  i32sq19 = Sq< int32_t,  19, lower, upper>;
template< double lower = details::lowestRealVMin< int32_t,  20>(), double upper = details::highestRealVMax< int32_t,  20>() > using  i32sq20 = Sq< int32_t,  20, lower, upper>;
template< double lower = details::lowestRealVMin< int32_t,  21>(), double upper = details::highestRealVMax< int32_t,  21>() > using  i32sq21 = Sq< int32_t,  21, lower, upper>;
template< double lower = details::lowestRealVMin< int32_t,  22>(), double upper = details::highestRealVMax< int32_t,  22>() > using  i32sq22 = Sq< int32_t,  22, lower, upper>;
template< double lower = details::lowestRealVMin< int32_t,  23>(), double upper = details::highestRealVMax< int32_t,  23>() > using  i32sq23 = Sq< int32_t,  23, lower, upper>;
template< double lower = details::lowestRealVMin< int32_t,  24>(), double upper = details::highestRealVMax< int32_t,  24>() > using  i32sq24 = Sq< int32_t,  24, lower, upper>;
template< double lower = details::lowestRealVMin< int32_t,  25>(), double upper = details::highestRealVMax< int32_t,  25>() > using  i32sq25 = Sq< int32_t,  25, lower, upper>;
template< double lower = details::lowestRealVMin< int32_t,  26>(), double upper = details::highestRealVMax< int32_t,  26>() > using  i32sq26 = Sq< int32_t,  26, lower, upper>;
template< double lower = details::lowestRealVMin< int32_t,  27>(), double upper = details::highestRealVMax< int32_t,  27>() > using  i32sq27 = Sq< int32_t,  27, lower, upper>;
template< double lower = details::lowestRealVMin< int32_t,  28>(), double upper = details::highestRealVMax< int32_t,  28>() > using  i32sq28 = Sq< int32_t,  28, lower, upper>;
template< double lower = details::lowestRealVMin< int32_t,  29>(), double upper = details::highestRealVMax< int32_t,  29>() > using  i32sq29 = Sq< int32_t,  29, lower, upper>;
template< double lower = details::lowestRealVMin< int32_t,  30>(), double upper = details::highestRealVMax< int32_t,  30>() > using  i32sq30 = Sq< int32_t,  30, lower, upper>;
template< double lower = details::lowestRealVMin< int32_t,  31>(), double upper = details::highestRealVMax< int32_t,  31>() > using  i32sq31 = Sq< int32_t,  31, lower, upper>;
template< double lower = details::lowestRealVMin<uint32_t, -16>(), double upper = details::highestRealVMax<uint32_t, -16>() > using u32sqm16 = Sq<uint32_t, -16, lower, upper>;
template< double lower = details::lowestRealVMin<uint32_t, -15>(), double upper = details::highestRealVMax<uint32_t, -15>() > using u32sqm15 = Sq<uint32_t, -15, lower, upper>;
template< double lower = details::lowestRealVMin<uint32_t, -14>(), double upper = details::highestRealVMax<uint32_t, -14>() > using u32sqm14 = Sq<uint32_t, -14, lower, upper>;
template< double lower = details::lowestRealVMin<uint32_t, -13>(), double upper = details::highestRealVMax<uint32_t, -13>() > using u32sqm13 = Sq<uint32_t, -13, lower, upper>;
template< double lower = details::lowestRealVMin<uint32_t, -12>(), double upper = details::highestRealVMax<uint32_t, -12>() > using u32sqm12 = Sq<uint32_t, -12, lower, upper>;
template< double lower = details::lowestRealVMin<uint32_t, -11>(), double upper = details::highestRealVMax<uint32_t, -11>() > using u32sqm11 = Sq<uint32_t, -11, lower, upper>;
template< double lower = details::lowestRealVMin<uint32_t, -10>(), double upper = details::highestRealVMax<uint32_t, -10>() > using u32sqm10 = Sq<uint32_t, -10, lower, upper>;
template< double lower = details::lowestRealVMin<uint32_t,  -9>(), double upper = details::highestRealVMax<uint32_t,  -9>() > using  u32sqm9 = Sq<uint32_t,  -9, lower, upper>;
template< double lower = details::lowestRealVMin<uint32_t,  -8>(), double upper = details::highestRealVMax<uint32_t,  -8>() > using  u32sqm8 = Sq<uint32_t,  -8, lower, upper>;
template< double lower = details::lowestRealVMin<uint32_t,  -7>(), double upper = details::highestRealVMax<uint32_t,  -7>() > using  u32sqm7 = Sq<uint32_t,  -7, lower, upper>;
template< double lower = details::lowestRealVMin<uint32_t,  -6>(), double upper = details::highestRealVMax<uint32_t,  -6>() > using  u32sqm6 = Sq<uint32_t,  -6, lower, upper>;
template< double lower = details::lowestRealVMin<uint32_t,  -5>(), double upper = details::highestRealVMax<uint32_t,  -5>() > using  u32sqm5 = Sq<uint32_t,  -5, lower, upper>;
template< double lower = details::lowestRealVMin<uint32_t,  -4>(), double upper = details::highestRealVMax<uint32_t,  -4>() > using  u32sqm4 = Sq<uint32_t,  -4, lower, upper>;
template< double lower = details::lowestRealVMin<uint32_t,  -3>(), double upper = details::highestRealVMax<uint32_t,  -3>() > using  u32sqm3 = Sq<uint32_t,  -3, lower, upper>;
template< double lower = details::lowestRealVMin<uint32_t,  -2>(), double upper = details::highestRealVMax<uint32_t,  -2>() > using  u32sqm2 = Sq<uint32_t,  -2, lower, upper>;
template< double lower = details::lowestRealVMin<uint32_t,  -1>(), double upper = details::highestRealVMax<uint32_t,  -1>() > using  u32sqm1 = Sq<uint32_t,  -1, lower, upper>;
template< double lower = details::lowestRealVMin<uint32_t,   0>(), double upper = details::highestRealVMax<uint32_t,   0>() > using   u32sq0 = Sq<uint32_t,   0, lower, upper>;
template< double lower = details::lowestRealVMin<uint32_t,   1>(), double upper = details::highestRealVMax<uint32_t,   1>() > using   u32sq1 = Sq<uint32_t,   1, lower, upper>;
template< double lower = details::lowestRealVMin<uint32_t,   2>(), double upper = details::highestRealVMax<uint32_t,   2>() > using   u32sq2 = Sq<uint32_t,   2, lower, upper>;
template< double lower = details::lowestRealVMin<uint32_t,   3>(), double upper = details::highestRealVMax<uint32_t,   3>() > using   u32sq3 = Sq<uint32_t,   3, lower, upper>;
template< double lower = details::lowestRealVMin<uint32_t,   4>(), double upper = details::highestRealVMax<uint32_t,   4>() > using   u32sq4 = Sq<uint32_t,   4, lower, upper>;
template< double lower = details::lowestRealVMin<uint32_t,   5>(), double upper = details::highestRealVMax<uint32_t,   5>() > using   u32sq5 = Sq<uint32_t,   5, lower, upper>;
template< double lower = details::lowestRealVMin<uint32_t,   6>(), double upper = details::highestRealVMax<uint32_t,   6>() > using   u32sq6 = Sq<uint32_t,   6, lower, upper>;
template< double lower = details::lowestRealVMin<uint32_t,   7>(), double upper = details::highestRealVMax<uint32_t,   7>() > using   u32sq7 = Sq<uint32_t,   7, lower, upper>;
template< double lower = details::lowestRealVMin<uint32_t,   8>(), double upper = details::highestRealVMax<uint32_t,   8>() > using   u32sq8 = Sq<uint32_t,   8, lower, upper>;
template< double lower = details::lowestRealVMin<uint32_t,   9>(), double upper = details::highestRealVMax<uint32_t,   9>() > using   u32sq9 = Sq<uint32_t,   9, lower, upper>;
template< double lower = details::lowestRealVMin<uint32_t,  10>(), double upper = details::highestRealVMax<uint32_t,  10>() > using  u32sq10 = Sq<uint32_t,  10, lower, upper>;
template< double lower = details::lowestRealVMin<uint32_t,  11>(), double upper = details::highestRealVMax<uint32_t,  11>() > using  u32sq11 = Sq<uint32_t,  11, lower, upper>;
template< double lower = details::lowestRealVMin<uint32_t,  12>(), double upper = details::highestRealVMax<uint32_t,  12>() > using  u32sq12 = Sq<uint32_t,  12, lower, upper>;
template< double lower = details::lowestRealVMin<uint32_t,  13>(), double upper = details::highestRealVMax<uint32_t,  13>() > using  u32sq13 = Sq<uint32_t,  13, lower, upper>;
template< double lower = details::lowestRealVMin<uint32_t,  14>(), double upper = details::highestRealVMax<uint32_t,  14>() > using  u32sq14 = Sq<uint32_t,  14, lower, upper>;
template< double lower = details::lowestRealVMin<uint32_t,  15>(), double upper = details::highestRealVMax<uint32_t,  15>() > using  u32sq15 = Sq<uint32_t,  15, lower, upper>;
template< double lower = details::lowestRealVMin<uint32_t,  16>(), double upper = details::highestRealVMax<uint32_t,  16>() > using  u32sq16 = Sq<uint32_t,  16, lower, upper>;
template< double lower = details::lowestRealVMin<uint32_t,  17>(), double upper = details::highestRealVMax<uint32_t,  17>() > using  u32sq17 = Sq<uint32_t,  17, lower, upper>;
template< double lower = details::lowestRealVMin<uint32_t,  18>(), double upper = details::highestRealVMax<uint32_t,  18>() > using  u32sq18 = Sq<uint32_t,  18, lower, upper>;
template< double lower = details::lowestRealVMin<uint32_t,  19>(), double upper = details::highestRealVMax<uint32_t,  19>() > using  u32sq19 = Sq<uint32_t,  19, lower, upper>;
template< double lower = details::lowestRealVMin<uint32_t,  20>(), double upper = details::highestRealVMax<uint32_t,  20>() > using  u32sq20 = Sq<uint32_t,  20, lower, upper>;
template< double lower = details::lowestRealVMin<uint32_t,  21>(), double upper = details::highestRealVMax<uint32_t,  21>() > using  u32sq21 = Sq<uint32_t,  21, lower, upper>;
template< double lower = details::lowestRealVMin<uint32_t,  22>(), double upper = details::highestRealVMax<uint32_t,  22>() > using  u32sq22 = Sq<uint32_t,  22, lower, upper>;
template< double lower = details::lowestRealVMin<uint32_t,  23>(), double upper = details::highestRealVMax<uint32_t,  23>() > using  u32sq23 = Sq<uint32_t,  23, lower, upper>;
template< double lower = details::lowestRealVMin<uint32_t,  24>(), double upper = details::highestRealVMax<uint32_t,  24>() > using  u32sq24 = Sq<uint32_t,  24, lower, upper>;
template< double lower = details::lowestRealVMin<uint32_t,  25>(), double upper = details::highestRealVMax<uint32_t,  25>() > using  u32sq25 = Sq<uint32_t,  25, lower, upper>;
template< double lower = details::lowestRealVMin<uint32_t,  26>(), double upper = details::highestRealVMax<uint32_t,  26>() > using  u32sq26 = Sq<uint32_t,  26, lower, upper>;
template< double lower = details::lowestRealVMin<uint32_t,  27>(), double upper = details::highestRealVMax<uint32_t,  27>() > using  u32sq27 = Sq<uint32_t,  27, lower, upper>;
template< double lower = details::lowestRealVMin<uint32_t,  28>(), double upper = details::highestRealVMax<uint32_t,  28>() > using  u32sq28 = Sq<uint32_t,  28, lower, upper>;
template< double lower = details::lowestRealVMin<uint32_t,  29>(), double upper = details::highestRealVMax<uint32_t,  29>() > using  u32sq29 = Sq<uint32_t,  29, lower, upper>;
template< double lower = details::lowestRealVMin<uint32_t,  30>(), double upper = details::highestRealVMax<uint32_t,  30>() > using  u32sq30 = Sq<uint32_t,  30, lower, upper>;
template< double lower = details::lowestRealVMin<uint32_t,  31>(), double upper = details::highestRealVMax<uint32_t,  31>() > using  u32sq31 = Sq<uint32_t,  31, lower, upper>;

/* literal operators */

template< char ...chars > consteval auto operator ""_i8sqm4()   { return sqFromLiteral<  i8sqm4<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i8sqm3()   { return sqFromLiteral<  i8sqm3<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i8sqm2()   { return sqFromLiteral<  i8sqm2<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i8sqm1()   { return sqFromLiteral<  i8sqm1<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i8sq0()    { return sqFromLiteral<   i8sq0<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i8sq1()    { return sqFromLiteral<   i8sq1<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i8sq2()    { return sqFromLiteral<   i8sq2<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i8sq3()    { return sqFromLiteral<   i8sq3<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i8sq4()    { return sqFromLiteral<   i8sq4<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i8sq5()    { return sqFromLiteral<   i8sq5<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i8sq6()    { return sqFromLiteral<   i8sq6<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i8sq7()    { return sqFromLiteral<   i8sq7<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u8sqm4()   { return sqFromLiteral<  u8sqm4<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u8sqm3()   { return sqFromLiteral<  u8sqm3<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u8sqm2()   { return sqFromLiteral<  u8sqm2<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u8sqm1()   { return sqFromLiteral<  u8sqm1<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u8sq0()    { return sqFromLiteral<   u8sq0<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u8sq1()    { return sqFromLiteral<   u8sq1<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u8sq2()    { return sqFromLiteral<   u8sq2<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u8sq3()    { return sqFromLiteral<   u8sq3<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u8sq4()    { return sqFromLiteral<   u8sq4<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u8sq5()    { return sqFromLiteral<   u8sq5<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u8sq6()    { return sqFromLiteral<   u8sq6<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u8sq7()    { return sqFromLiteral<   u8sq7<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i16sqm8()  { return sqFromLiteral< i16sqm8<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i16sqm7()  { return sqFromLiteral< i16sqm7<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i16sqm6()  { return sqFromLiteral< i16sqm6<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i16sqm5()  { return sqFromLiteral< i16sqm5<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i16sqm4()  { return sqFromLiteral< i16sqm4<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i16sqm3()  { return sqFromLiteral< i16sqm3<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i16sqm2()  { return sqFromLiteral< i16sqm2<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i16sqm1()  { return sqFromLiteral< i16sqm1<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i16sq0()   { return sqFromLiteral<  i16sq0<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i16sq1()   { return sqFromLiteral<  i16sq1<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i16sq2()   { return sqFromLiteral<  i16sq2<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i16sq3()   { return sqFromLiteral<  i16sq3<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i16sq4()   { return sqFromLiteral<  i16sq4<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i16sq5()   { return sqFromLiteral<  i16sq5<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i16sq6()   { return sqFromLiteral<  i16sq6<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i16sq7()   { return sqFromLiteral<  i16sq7<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i16sq8()   { return sqFromLiteral<  i16sq8<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i16sq9()   { return sqFromLiteral<  i16sq9<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i16sq10()  { return sqFromLiteral< i16sq10<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i16sq11()  { return sqFromLiteral< i16sq11<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i16sq12()  { return sqFromLiteral< i16sq12<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i16sq13()  { return sqFromLiteral< i16sq13<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i16sq14()  { return sqFromLiteral< i16sq14<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i16sq15()  { return sqFromLiteral< i16sq15<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u16sqm8()  { return sqFromLiteral< u16sqm8<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u16sqm7()  { return sqFromLiteral< u16sqm7<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u16sqm6()  { return sqFromLiteral< u16sqm6<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u16sqm5()  { return sqFromLiteral< u16sqm5<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u16sqm4()  { return sqFromLiteral< u16sqm4<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u16sqm3()  { return sqFromLiteral< u16sqm3<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u16sqm2()  { return sqFromLiteral< u16sqm2<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u16sqm1()  { return sqFromLiteral< u16sqm1<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u16sq0()   { return sqFromLiteral<  u16sq0<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u16sq1()   { return sqFromLiteral<  u16sq1<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u16sq2()   { return sqFromLiteral<  u16sq2<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u16sq3()   { return sqFromLiteral<  u16sq3<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u16sq4()   { return sqFromLiteral<  u16sq4<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u16sq5()   { return sqFromLiteral<  u16sq5<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u16sq6()   { return sqFromLiteral<  u16sq6<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u16sq7()   { return sqFromLiteral<  u16sq7<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u16sq8()   { return sqFromLiteral<  u16sq8<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u16sq9()   { return sqFromLiteral<  u16sq9<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u16sq10()  { return sqFromLiteral< u16sq10<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u16sq11()  { return sqFromLiteral< u16sq11<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u16sq12()  { return sqFromLiteral< u16sq12<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u16sq13()  { return sqFromLiteral< u16sq13<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u16sq14()  { return sqFromLiteral< u16sq14<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u16sq15()  { return sqFromLiteral< u16sq15<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i32sqm16() { return sqFromLiteral<i32sqm16<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i32sqm15() { return sqFromLiteral<i32sqm15<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i32sqm14() { return sqFromLiteral<i32sqm14<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i32sqm13() { return sqFromLiteral<i32sqm13<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i32sqm12() { return sqFromLiteral<i32sqm12<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i32sqm11() { return sqFromLiteral<i32sqm11<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i32sqm10() { return sqFromLiteral<i32sqm10<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i32sqm9()  { return sqFromLiteral< i32sqm9<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i32sqm8()  { return sqFromLiteral< i32sqm8<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i32sqm7()  { return sqFromLiteral< i32sqm7<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i32sqm6()  { return sqFromLiteral< i32sqm6<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i32sqm5()  { return sqFromLiteral< i32sqm5<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i32sqm4()  { return sqFromLiteral< i32sqm4<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i32sqm3()  { return sqFromLiteral< i32sqm3<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i32sqm2()  { return sqFromLiteral< i32sqm2<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i32sqm1()  { return sqFromLiteral< i32sqm1<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i32sq0()   { return sqFromLiteral<  i32sq0<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i32sq1()   { return sqFromLiteral<  i32sq1<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i32sq2()   { return sqFromLiteral<  i32sq2<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i32sq3()   { return sqFromLiteral<  i32sq3<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i32sq4()   { return sqFromLiteral<  i32sq4<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i32sq5()   { return sqFromLiteral<  i32sq5<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i32sq6()   { return sqFromLiteral<  i32sq6<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i32sq7()   { return sqFromLiteral<  i32sq7<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i32sq8()   { return sqFromLiteral<  i32sq8<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i32sq9()   { return sqFromLiteral<  i32sq9<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i32sq10()  { return sqFromLiteral< i32sq10<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i32sq11()  { return sqFromLiteral< i32sq11<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i32sq12()  { return sqFromLiteral< i32sq12<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i32sq13()  { return sqFromLiteral< i32sq13<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i32sq14()  { return sqFromLiteral< i32sq14<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i32sq15()  { return sqFromLiteral< i32sq15<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i32sq16()  { return sqFromLiteral< i32sq16<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i32sq17()  { return sqFromLiteral< i32sq17<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i32sq18()  { return sqFromLiteral< i32sq18<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i32sq19()  { return sqFromLiteral< i32sq19<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i32sq20()  { return sqFromLiteral< i32sq20<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i32sq21()  { return sqFromLiteral< i32sq21<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i32sq22()  { return sqFromLiteral< i32sq22<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i32sq23()  { return sqFromLiteral< i32sq23<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i32sq24()  { return sqFromLiteral< i32sq24<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i32sq25()  { return sqFromLiteral< i32sq25<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i32sq26()  { return sqFromLiteral< i32sq26<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i32sq27()  { return sqFromLiteral< i32sq27<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i32sq28()  { return sqFromLiteral< i32sq28<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i32sq29()  { return sqFromLiteral< i32sq29<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i32sq30()  { return sqFromLiteral< i32sq30<>, chars...>(); }
template< char ...chars > consteval auto operator ""_i32sq31()  { return sqFromLiteral< i32sq31<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u32sqm16() { return sqFromLiteral<u32sqm16<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u32sqm15() { return sqFromLiteral<u32sqm15<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u32sqm14() { return sqFromLiteral<u32sqm14<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u32sqm13() { return sqFromLiteral<u32sqm13<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u32sqm12() { return sqFromLiteral<u32sqm12<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u32sqm11() { return sqFromLiteral<u32sqm11<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u32sqm10() { return sqFromLiteral<u32sqm10<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u32sqm9()  { return sqFromLiteral< u32sqm9<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u32sqm8()  { return sqFromLiteral< u32sqm8<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u32sqm7()  { return sqFromLiteral< u32sqm7<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u32sqm6()  { return sqFromLiteral< u32sqm6<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u32sqm5()  { return sqFromLiteral< u32sqm5<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u32sqm4()  { return sqFromLiteral< u32sqm4<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u32sqm3()  { return sqFromLiteral< u32sqm3<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u32sqm2()  { return sqFromLiteral< u32sqm2<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u32sqm1()  { return sqFromLiteral< u32sqm1<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u32sq0()   { return sqFromLiteral<  u32sq0<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u32sq1()   { return sqFromLiteral<  u32sq1<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u32sq2()   { return sqFromLiteral<  u32sq2<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u32sq3()   { return sqFromLiteral<  u32sq3<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u32sq4()   { return sqFromLiteral<  u32sq4<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u32sq5()   { return sqFromLiteral<  u32sq5<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u32sq6()   { return sqFromLiteral<  u32sq6<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u32sq7()   { return sqFromLiteral<  u32sq7<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u32sq8()   { return sqFromLiteral<  u32sq8<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u32sq9()   { return sqFromLiteral<  u32sq9<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u32sq10()  { return sqFromLiteral< u32sq10<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u32sq11()  { return sqFromLiteral< u32sq11<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u32sq12()  { return sqFromLiteral< u32sq12<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u32sq13()  { return sqFromLiteral< u32sq13<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u32sq14()  { return sqFromLiteral< u32sq14<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u32sq15()  { return sqFromLiteral< u32sq15<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u32sq16()  { return sqFromLiteral< u32sq16<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u32sq17()  { return sqFromLiteral< u32sq17<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u32sq18()  { return sqFromLiteral< u32sq18<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u32sq19()  { return sqFromLiteral< u32sq19<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u32sq20()  { return sqFromLiteral< u32sq20<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u32sq21()  { return sqFromLiteral< u32sq21<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u32sq22()  { return sqFromLiteral< u32sq22<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u32sq23()  { return sqFromLiteral< u32sq23<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u32sq24()  { return sqFromLiteral< u32sq24<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u32sq25()  { return sqFromLiteral< u32sq25<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u32sq26()  { return sqFromLiteral< u32sq26<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u32sq27()  { return sqFromLiteral< u32sq27<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u32sq28()  { return sqFromLiteral< u32sq28<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u32sq29()  { return sqFromLiteral< u32sq29<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u32sq30()  { return sqFromLiteral< u32sq30<>, chars...>(); }
template< char ...chars > consteval auto operator ""_u32sq31()  { return sqFromLiteral< u32sq31<>, chars...>(); }

/**\}*/
}

#endif
// EOF
