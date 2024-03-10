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

template< double lower = fpm::detail::lowestRealVMin<  int8_t,  -4>(), double upper = fpm::detail::highestRealVMax<  int8_t,  -4>() > using   i8sqm4 = Sq<  int8_t,  -4, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<  int8_t,  -3>(), double upper = fpm::detail::highestRealVMax<  int8_t,  -3>() > using   i8sqm3 = Sq<  int8_t,  -3, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<  int8_t,  -2>(), double upper = fpm::detail::highestRealVMax<  int8_t,  -2>() > using   i8sqm2 = Sq<  int8_t,  -2, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<  int8_t,  -1>(), double upper = fpm::detail::highestRealVMax<  int8_t,  -1>() > using   i8sqm1 = Sq<  int8_t,  -1, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<  int8_t,   0>(), double upper = fpm::detail::highestRealVMax<  int8_t,   0>() > using    i8sq0 = Sq<  int8_t,   0, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<  int8_t,   1>(), double upper = fpm::detail::highestRealVMax<  int8_t,   1>() > using    i8sq1 = Sq<  int8_t,   1, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<  int8_t,   2>(), double upper = fpm::detail::highestRealVMax<  int8_t,   2>() > using    i8sq2 = Sq<  int8_t,   2, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<  int8_t,   3>(), double upper = fpm::detail::highestRealVMax<  int8_t,   3>() > using    i8sq3 = Sq<  int8_t,   3, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<  int8_t,   4>(), double upper = fpm::detail::highestRealVMax<  int8_t,   4>() > using    i8sq4 = Sq<  int8_t,   4, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<  int8_t,   5>(), double upper = fpm::detail::highestRealVMax<  int8_t,   5>() > using    i8sq5 = Sq<  int8_t,   5, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<  int8_t,   6>(), double upper = fpm::detail::highestRealVMax<  int8_t,   6>() > using    i8sq6 = Sq<  int8_t,   6, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<  int8_t,   7>(), double upper = fpm::detail::highestRealVMax<  int8_t,   7>() > using    i8sq7 = Sq<  int8_t,   7, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< uint8_t,  -4>(), double upper = fpm::detail::highestRealVMax< uint8_t,  -4>() > using   u8sqm4 = Sq< uint8_t,  -4, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< uint8_t,  -3>(), double upper = fpm::detail::highestRealVMax< uint8_t,  -3>() > using   u8sqm3 = Sq< uint8_t,  -3, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< uint8_t,  -2>(), double upper = fpm::detail::highestRealVMax< uint8_t,  -2>() > using   u8sqm2 = Sq< uint8_t,  -2, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< uint8_t,  -1>(), double upper = fpm::detail::highestRealVMax< uint8_t,  -1>() > using   u8sqm1 = Sq< uint8_t,  -1, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< uint8_t,   0>(), double upper = fpm::detail::highestRealVMax< uint8_t,   0>() > using    u8sq0 = Sq< uint8_t,   0, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< uint8_t,   1>(), double upper = fpm::detail::highestRealVMax< uint8_t,   1>() > using    u8sq1 = Sq< uint8_t,   1, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< uint8_t,   2>(), double upper = fpm::detail::highestRealVMax< uint8_t,   2>() > using    u8sq2 = Sq< uint8_t,   2, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< uint8_t,   3>(), double upper = fpm::detail::highestRealVMax< uint8_t,   3>() > using    u8sq3 = Sq< uint8_t,   3, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< uint8_t,   4>(), double upper = fpm::detail::highestRealVMax< uint8_t,   4>() > using    u8sq4 = Sq< uint8_t,   4, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< uint8_t,   5>(), double upper = fpm::detail::highestRealVMax< uint8_t,   5>() > using    u8sq5 = Sq< uint8_t,   5, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< uint8_t,   6>(), double upper = fpm::detail::highestRealVMax< uint8_t,   6>() > using    u8sq6 = Sq< uint8_t,   6, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< uint8_t,   7>(), double upper = fpm::detail::highestRealVMax< uint8_t,   7>() > using    u8sq7 = Sq< uint8_t,   7, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int16_t,  -8>(), double upper = fpm::detail::highestRealVMax< int16_t,  -8>() > using  i16sqm8 = Sq< int16_t,  -8, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int16_t,  -7>(), double upper = fpm::detail::highestRealVMax< int16_t,  -7>() > using  i16sqm7 = Sq< int16_t,  -7, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int16_t,  -6>(), double upper = fpm::detail::highestRealVMax< int16_t,  -6>() > using  i16sqm6 = Sq< int16_t,  -6, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int16_t,  -5>(), double upper = fpm::detail::highestRealVMax< int16_t,  -5>() > using  i16sqm5 = Sq< int16_t,  -5, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int16_t,  -4>(), double upper = fpm::detail::highestRealVMax< int16_t,  -4>() > using  i16sqm4 = Sq< int16_t,  -4, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int16_t,  -3>(), double upper = fpm::detail::highestRealVMax< int16_t,  -3>() > using  i16sqm3 = Sq< int16_t,  -3, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int16_t,  -2>(), double upper = fpm::detail::highestRealVMax< int16_t,  -2>() > using  i16sqm2 = Sq< int16_t,  -2, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int16_t,  -1>(), double upper = fpm::detail::highestRealVMax< int16_t,  -1>() > using  i16sqm1 = Sq< int16_t,  -1, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int16_t,   0>(), double upper = fpm::detail::highestRealVMax< int16_t,   0>() > using   i16sq0 = Sq< int16_t,   0, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int16_t,   1>(), double upper = fpm::detail::highestRealVMax< int16_t,   1>() > using   i16sq1 = Sq< int16_t,   1, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int16_t,   2>(), double upper = fpm::detail::highestRealVMax< int16_t,   2>() > using   i16sq2 = Sq< int16_t,   2, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int16_t,   3>(), double upper = fpm::detail::highestRealVMax< int16_t,   3>() > using   i16sq3 = Sq< int16_t,   3, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int16_t,   4>(), double upper = fpm::detail::highestRealVMax< int16_t,   4>() > using   i16sq4 = Sq< int16_t,   4, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int16_t,   5>(), double upper = fpm::detail::highestRealVMax< int16_t,   5>() > using   i16sq5 = Sq< int16_t,   5, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int16_t,   6>(), double upper = fpm::detail::highestRealVMax< int16_t,   6>() > using   i16sq6 = Sq< int16_t,   6, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int16_t,   7>(), double upper = fpm::detail::highestRealVMax< int16_t,   7>() > using   i16sq7 = Sq< int16_t,   7, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int16_t,   8>(), double upper = fpm::detail::highestRealVMax< int16_t,   8>() > using   i16sq8 = Sq< int16_t,   8, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int16_t,   9>(), double upper = fpm::detail::highestRealVMax< int16_t,   9>() > using   i16sq9 = Sq< int16_t,   9, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int16_t,  10>(), double upper = fpm::detail::highestRealVMax< int16_t,  10>() > using  i16sq10 = Sq< int16_t,  10, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int16_t,  11>(), double upper = fpm::detail::highestRealVMax< int16_t,  11>() > using  i16sq11 = Sq< int16_t,  11, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int16_t,  12>(), double upper = fpm::detail::highestRealVMax< int16_t,  12>() > using  i16sq12 = Sq< int16_t,  12, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int16_t,  13>(), double upper = fpm::detail::highestRealVMax< int16_t,  13>() > using  i16sq13 = Sq< int16_t,  13, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int16_t,  14>(), double upper = fpm::detail::highestRealVMax< int16_t,  14>() > using  i16sq14 = Sq< int16_t,  14, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int16_t,  15>(), double upper = fpm::detail::highestRealVMax< int16_t,  15>() > using  i16sq15 = Sq< int16_t,  15, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint16_t,  -8>(), double upper = fpm::detail::highestRealVMax<uint16_t,  -8>() > using  u16sqm8 = Sq<uint16_t,  -8, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint16_t,  -7>(), double upper = fpm::detail::highestRealVMax<uint16_t,  -7>() > using  u16sqm7 = Sq<uint16_t,  -7, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint16_t,  -6>(), double upper = fpm::detail::highestRealVMax<uint16_t,  -6>() > using  u16sqm6 = Sq<uint16_t,  -6, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint16_t,  -5>(), double upper = fpm::detail::highestRealVMax<uint16_t,  -5>() > using  u16sqm5 = Sq<uint16_t,  -5, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint16_t,  -4>(), double upper = fpm::detail::highestRealVMax<uint16_t,  -4>() > using  u16sqm4 = Sq<uint16_t,  -4, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint16_t,  -3>(), double upper = fpm::detail::highestRealVMax<uint16_t,  -3>() > using  u16sqm3 = Sq<uint16_t,  -3, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint16_t,  -2>(), double upper = fpm::detail::highestRealVMax<uint16_t,  -2>() > using  u16sqm2 = Sq<uint16_t,  -2, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint16_t,  -1>(), double upper = fpm::detail::highestRealVMax<uint16_t,  -1>() > using  u16sqm1 = Sq<uint16_t,  -1, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint16_t,   0>(), double upper = fpm::detail::highestRealVMax<uint16_t,   0>() > using   u16sq0 = Sq<uint16_t,   0, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint16_t,   1>(), double upper = fpm::detail::highestRealVMax<uint16_t,   1>() > using   u16sq1 = Sq<uint16_t,   1, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint16_t,   2>(), double upper = fpm::detail::highestRealVMax<uint16_t,   2>() > using   u16sq2 = Sq<uint16_t,   2, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint16_t,   3>(), double upper = fpm::detail::highestRealVMax<uint16_t,   3>() > using   u16sq3 = Sq<uint16_t,   3, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint16_t,   4>(), double upper = fpm::detail::highestRealVMax<uint16_t,   4>() > using   u16sq4 = Sq<uint16_t,   4, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint16_t,   5>(), double upper = fpm::detail::highestRealVMax<uint16_t,   5>() > using   u16sq5 = Sq<uint16_t,   5, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint16_t,   6>(), double upper = fpm::detail::highestRealVMax<uint16_t,   6>() > using   u16sq6 = Sq<uint16_t,   6, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint16_t,   7>(), double upper = fpm::detail::highestRealVMax<uint16_t,   7>() > using   u16sq7 = Sq<uint16_t,   7, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint16_t,   8>(), double upper = fpm::detail::highestRealVMax<uint16_t,   8>() > using   u16sq8 = Sq<uint16_t,   8, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint16_t,   9>(), double upper = fpm::detail::highestRealVMax<uint16_t,   9>() > using   u16sq9 = Sq<uint16_t,   9, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint16_t,  10>(), double upper = fpm::detail::highestRealVMax<uint16_t,  10>() > using  u16sq10 = Sq<uint16_t,  10, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint16_t,  11>(), double upper = fpm::detail::highestRealVMax<uint16_t,  11>() > using  u16sq11 = Sq<uint16_t,  11, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint16_t,  12>(), double upper = fpm::detail::highestRealVMax<uint16_t,  12>() > using  u16sq12 = Sq<uint16_t,  12, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint16_t,  13>(), double upper = fpm::detail::highestRealVMax<uint16_t,  13>() > using  u16sq13 = Sq<uint16_t,  13, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint16_t,  14>(), double upper = fpm::detail::highestRealVMax<uint16_t,  14>() > using  u16sq14 = Sq<uint16_t,  14, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint16_t,  15>(), double upper = fpm::detail::highestRealVMax<uint16_t,  15>() > using  u16sq15 = Sq<uint16_t,  15, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int32_t, -16>(), double upper = fpm::detail::highestRealVMax< int32_t, -16>() > using i32sqm16 = Sq< int32_t, -16, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int32_t, -15>(), double upper = fpm::detail::highestRealVMax< int32_t, -15>() > using i32sqm15 = Sq< int32_t, -15, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int32_t, -14>(), double upper = fpm::detail::highestRealVMax< int32_t, -14>() > using i32sqm14 = Sq< int32_t, -14, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int32_t, -13>(), double upper = fpm::detail::highestRealVMax< int32_t, -13>() > using i32sqm13 = Sq< int32_t, -13, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int32_t, -12>(), double upper = fpm::detail::highestRealVMax< int32_t, -12>() > using i32sqm12 = Sq< int32_t, -12, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int32_t, -11>(), double upper = fpm::detail::highestRealVMax< int32_t, -11>() > using i32sqm11 = Sq< int32_t, -11, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int32_t, -10>(), double upper = fpm::detail::highestRealVMax< int32_t, -10>() > using i32sqm10 = Sq< int32_t, -10, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int32_t,  -9>(), double upper = fpm::detail::highestRealVMax< int32_t,  -9>() > using  i32sqm9 = Sq< int32_t,  -9, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int32_t,  -8>(), double upper = fpm::detail::highestRealVMax< int32_t,  -8>() > using  i32sqm8 = Sq< int32_t,  -8, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int32_t,  -7>(), double upper = fpm::detail::highestRealVMax< int32_t,  -7>() > using  i32sqm7 = Sq< int32_t,  -7, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int32_t,  -6>(), double upper = fpm::detail::highestRealVMax< int32_t,  -6>() > using  i32sqm6 = Sq< int32_t,  -6, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int32_t,  -5>(), double upper = fpm::detail::highestRealVMax< int32_t,  -5>() > using  i32sqm5 = Sq< int32_t,  -5, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int32_t,  -4>(), double upper = fpm::detail::highestRealVMax< int32_t,  -4>() > using  i32sqm4 = Sq< int32_t,  -4, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int32_t,  -3>(), double upper = fpm::detail::highestRealVMax< int32_t,  -3>() > using  i32sqm3 = Sq< int32_t,  -3, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int32_t,  -2>(), double upper = fpm::detail::highestRealVMax< int32_t,  -2>() > using  i32sqm2 = Sq< int32_t,  -2, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int32_t,  -1>(), double upper = fpm::detail::highestRealVMax< int32_t,  -1>() > using  i32sqm1 = Sq< int32_t,  -1, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int32_t,   0>(), double upper = fpm::detail::highestRealVMax< int32_t,   0>() > using   i32sq0 = Sq< int32_t,   0, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int32_t,   1>(), double upper = fpm::detail::highestRealVMax< int32_t,   1>() > using   i32sq1 = Sq< int32_t,   1, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int32_t,   2>(), double upper = fpm::detail::highestRealVMax< int32_t,   2>() > using   i32sq2 = Sq< int32_t,   2, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int32_t,   3>(), double upper = fpm::detail::highestRealVMax< int32_t,   3>() > using   i32sq3 = Sq< int32_t,   3, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int32_t,   4>(), double upper = fpm::detail::highestRealVMax< int32_t,   4>() > using   i32sq4 = Sq< int32_t,   4, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int32_t,   5>(), double upper = fpm::detail::highestRealVMax< int32_t,   5>() > using   i32sq5 = Sq< int32_t,   5, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int32_t,   6>(), double upper = fpm::detail::highestRealVMax< int32_t,   6>() > using   i32sq6 = Sq< int32_t,   6, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int32_t,   7>(), double upper = fpm::detail::highestRealVMax< int32_t,   7>() > using   i32sq7 = Sq< int32_t,   7, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int32_t,   8>(), double upper = fpm::detail::highestRealVMax< int32_t,   8>() > using   i32sq8 = Sq< int32_t,   8, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int32_t,   9>(), double upper = fpm::detail::highestRealVMax< int32_t,   9>() > using   i32sq9 = Sq< int32_t,   9, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int32_t,  10>(), double upper = fpm::detail::highestRealVMax< int32_t,  10>() > using  i32sq10 = Sq< int32_t,  10, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int32_t,  11>(), double upper = fpm::detail::highestRealVMax< int32_t,  11>() > using  i32sq11 = Sq< int32_t,  11, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int32_t,  12>(), double upper = fpm::detail::highestRealVMax< int32_t,  12>() > using  i32sq12 = Sq< int32_t,  12, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int32_t,  13>(), double upper = fpm::detail::highestRealVMax< int32_t,  13>() > using  i32sq13 = Sq< int32_t,  13, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int32_t,  14>(), double upper = fpm::detail::highestRealVMax< int32_t,  14>() > using  i32sq14 = Sq< int32_t,  14, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int32_t,  15>(), double upper = fpm::detail::highestRealVMax< int32_t,  15>() > using  i32sq15 = Sq< int32_t,  15, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int32_t,  16>(), double upper = fpm::detail::highestRealVMax< int32_t,  16>() > using  i32sq16 = Sq< int32_t,  16, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int32_t,  17>(), double upper = fpm::detail::highestRealVMax< int32_t,  17>() > using  i32sq17 = Sq< int32_t,  17, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int32_t,  18>(), double upper = fpm::detail::highestRealVMax< int32_t,  18>() > using  i32sq18 = Sq< int32_t,  18, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int32_t,  19>(), double upper = fpm::detail::highestRealVMax< int32_t,  19>() > using  i32sq19 = Sq< int32_t,  19, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int32_t,  20>(), double upper = fpm::detail::highestRealVMax< int32_t,  20>() > using  i32sq20 = Sq< int32_t,  20, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int32_t,  21>(), double upper = fpm::detail::highestRealVMax< int32_t,  21>() > using  i32sq21 = Sq< int32_t,  21, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int32_t,  22>(), double upper = fpm::detail::highestRealVMax< int32_t,  22>() > using  i32sq22 = Sq< int32_t,  22, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int32_t,  23>(), double upper = fpm::detail::highestRealVMax< int32_t,  23>() > using  i32sq23 = Sq< int32_t,  23, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int32_t,  24>(), double upper = fpm::detail::highestRealVMax< int32_t,  24>() > using  i32sq24 = Sq< int32_t,  24, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int32_t,  25>(), double upper = fpm::detail::highestRealVMax< int32_t,  25>() > using  i32sq25 = Sq< int32_t,  25, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int32_t,  26>(), double upper = fpm::detail::highestRealVMax< int32_t,  26>() > using  i32sq26 = Sq< int32_t,  26, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int32_t,  27>(), double upper = fpm::detail::highestRealVMax< int32_t,  27>() > using  i32sq27 = Sq< int32_t,  27, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int32_t,  28>(), double upper = fpm::detail::highestRealVMax< int32_t,  28>() > using  i32sq28 = Sq< int32_t,  28, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int32_t,  29>(), double upper = fpm::detail::highestRealVMax< int32_t,  29>() > using  i32sq29 = Sq< int32_t,  29, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int32_t,  30>(), double upper = fpm::detail::highestRealVMax< int32_t,  30>() > using  i32sq30 = Sq< int32_t,  30, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin< int32_t,  31>(), double upper = fpm::detail::highestRealVMax< int32_t,  31>() > using  i32sq31 = Sq< int32_t,  31, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint32_t, -16>(), double upper = fpm::detail::highestRealVMax<uint32_t, -16>() > using u32sqm16 = Sq<uint32_t, -16, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint32_t, -15>(), double upper = fpm::detail::highestRealVMax<uint32_t, -15>() > using u32sqm15 = Sq<uint32_t, -15, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint32_t, -14>(), double upper = fpm::detail::highestRealVMax<uint32_t, -14>() > using u32sqm14 = Sq<uint32_t, -14, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint32_t, -13>(), double upper = fpm::detail::highestRealVMax<uint32_t, -13>() > using u32sqm13 = Sq<uint32_t, -13, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint32_t, -12>(), double upper = fpm::detail::highestRealVMax<uint32_t, -12>() > using u32sqm12 = Sq<uint32_t, -12, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint32_t, -11>(), double upper = fpm::detail::highestRealVMax<uint32_t, -11>() > using u32sqm11 = Sq<uint32_t, -11, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint32_t, -10>(), double upper = fpm::detail::highestRealVMax<uint32_t, -10>() > using u32sqm10 = Sq<uint32_t, -10, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint32_t,  -9>(), double upper = fpm::detail::highestRealVMax<uint32_t,  -9>() > using  u32sqm9 = Sq<uint32_t,  -9, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint32_t,  -8>(), double upper = fpm::detail::highestRealVMax<uint32_t,  -8>() > using  u32sqm8 = Sq<uint32_t,  -8, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint32_t,  -7>(), double upper = fpm::detail::highestRealVMax<uint32_t,  -7>() > using  u32sqm7 = Sq<uint32_t,  -7, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint32_t,  -6>(), double upper = fpm::detail::highestRealVMax<uint32_t,  -6>() > using  u32sqm6 = Sq<uint32_t,  -6, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint32_t,  -5>(), double upper = fpm::detail::highestRealVMax<uint32_t,  -5>() > using  u32sqm5 = Sq<uint32_t,  -5, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint32_t,  -4>(), double upper = fpm::detail::highestRealVMax<uint32_t,  -4>() > using  u32sqm4 = Sq<uint32_t,  -4, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint32_t,  -3>(), double upper = fpm::detail::highestRealVMax<uint32_t,  -3>() > using  u32sqm3 = Sq<uint32_t,  -3, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint32_t,  -2>(), double upper = fpm::detail::highestRealVMax<uint32_t,  -2>() > using  u32sqm2 = Sq<uint32_t,  -2, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint32_t,  -1>(), double upper = fpm::detail::highestRealVMax<uint32_t,  -1>() > using  u32sqm1 = Sq<uint32_t,  -1, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint32_t,   0>(), double upper = fpm::detail::highestRealVMax<uint32_t,   0>() > using   u32sq0 = Sq<uint32_t,   0, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint32_t,   1>(), double upper = fpm::detail::highestRealVMax<uint32_t,   1>() > using   u32sq1 = Sq<uint32_t,   1, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint32_t,   2>(), double upper = fpm::detail::highestRealVMax<uint32_t,   2>() > using   u32sq2 = Sq<uint32_t,   2, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint32_t,   3>(), double upper = fpm::detail::highestRealVMax<uint32_t,   3>() > using   u32sq3 = Sq<uint32_t,   3, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint32_t,   4>(), double upper = fpm::detail::highestRealVMax<uint32_t,   4>() > using   u32sq4 = Sq<uint32_t,   4, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint32_t,   5>(), double upper = fpm::detail::highestRealVMax<uint32_t,   5>() > using   u32sq5 = Sq<uint32_t,   5, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint32_t,   6>(), double upper = fpm::detail::highestRealVMax<uint32_t,   6>() > using   u32sq6 = Sq<uint32_t,   6, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint32_t,   7>(), double upper = fpm::detail::highestRealVMax<uint32_t,   7>() > using   u32sq7 = Sq<uint32_t,   7, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint32_t,   8>(), double upper = fpm::detail::highestRealVMax<uint32_t,   8>() > using   u32sq8 = Sq<uint32_t,   8, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint32_t,   9>(), double upper = fpm::detail::highestRealVMax<uint32_t,   9>() > using   u32sq9 = Sq<uint32_t,   9, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint32_t,  10>(), double upper = fpm::detail::highestRealVMax<uint32_t,  10>() > using  u32sq10 = Sq<uint32_t,  10, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint32_t,  11>(), double upper = fpm::detail::highestRealVMax<uint32_t,  11>() > using  u32sq11 = Sq<uint32_t,  11, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint32_t,  12>(), double upper = fpm::detail::highestRealVMax<uint32_t,  12>() > using  u32sq12 = Sq<uint32_t,  12, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint32_t,  13>(), double upper = fpm::detail::highestRealVMax<uint32_t,  13>() > using  u32sq13 = Sq<uint32_t,  13, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint32_t,  14>(), double upper = fpm::detail::highestRealVMax<uint32_t,  14>() > using  u32sq14 = Sq<uint32_t,  14, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint32_t,  15>(), double upper = fpm::detail::highestRealVMax<uint32_t,  15>() > using  u32sq15 = Sq<uint32_t,  15, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint32_t,  16>(), double upper = fpm::detail::highestRealVMax<uint32_t,  16>() > using  u32sq16 = Sq<uint32_t,  16, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint32_t,  17>(), double upper = fpm::detail::highestRealVMax<uint32_t,  17>() > using  u32sq17 = Sq<uint32_t,  17, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint32_t,  18>(), double upper = fpm::detail::highestRealVMax<uint32_t,  18>() > using  u32sq18 = Sq<uint32_t,  18, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint32_t,  19>(), double upper = fpm::detail::highestRealVMax<uint32_t,  19>() > using  u32sq19 = Sq<uint32_t,  19, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint32_t,  20>(), double upper = fpm::detail::highestRealVMax<uint32_t,  20>() > using  u32sq20 = Sq<uint32_t,  20, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint32_t,  21>(), double upper = fpm::detail::highestRealVMax<uint32_t,  21>() > using  u32sq21 = Sq<uint32_t,  21, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint32_t,  22>(), double upper = fpm::detail::highestRealVMax<uint32_t,  22>() > using  u32sq22 = Sq<uint32_t,  22, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint32_t,  23>(), double upper = fpm::detail::highestRealVMax<uint32_t,  23>() > using  u32sq23 = Sq<uint32_t,  23, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint32_t,  24>(), double upper = fpm::detail::highestRealVMax<uint32_t,  24>() > using  u32sq24 = Sq<uint32_t,  24, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint32_t,  25>(), double upper = fpm::detail::highestRealVMax<uint32_t,  25>() > using  u32sq25 = Sq<uint32_t,  25, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint32_t,  26>(), double upper = fpm::detail::highestRealVMax<uint32_t,  26>() > using  u32sq26 = Sq<uint32_t,  26, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint32_t,  27>(), double upper = fpm::detail::highestRealVMax<uint32_t,  27>() > using  u32sq27 = Sq<uint32_t,  27, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint32_t,  28>(), double upper = fpm::detail::highestRealVMax<uint32_t,  28>() > using  u32sq28 = Sq<uint32_t,  28, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint32_t,  29>(), double upper = fpm::detail::highestRealVMax<uint32_t,  29>() > using  u32sq29 = Sq<uint32_t,  29, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint32_t,  30>(), double upper = fpm::detail::highestRealVMax<uint32_t,  30>() > using  u32sq30 = Sq<uint32_t,  30, lower, upper>;
template< double lower = fpm::detail::lowestRealVMin<uint32_t,  31>(), double upper = fpm::detail::highestRealVMax<uint32_t,  31>() > using  u32sq31 = Sq<uint32_t,  31, lower, upper>;

/* literal operators */

FPM_SQ_BIND_LITERAL(  i8sqm4<>, i8sqm4  )
FPM_SQ_BIND_LITERAL(  i8sqm3<>, i8sqm3  )
FPM_SQ_BIND_LITERAL(  i8sqm2<>, i8sqm2  )
FPM_SQ_BIND_LITERAL(  i8sqm1<>, i8sqm1  )
FPM_SQ_BIND_LITERAL(   i8sq0<>, i8sq0   )
FPM_SQ_BIND_LITERAL(   i8sq1<>, i8sq1   )
FPM_SQ_BIND_LITERAL(   i8sq2<>, i8sq2   )
FPM_SQ_BIND_LITERAL(   i8sq3<>, i8sq3   )
FPM_SQ_BIND_LITERAL(   i8sq4<>, i8sq4   )
FPM_SQ_BIND_LITERAL(   i8sq5<>, i8sq5   )
FPM_SQ_BIND_LITERAL(   i8sq6<>, i8sq6   )
FPM_SQ_BIND_LITERAL(   i8sq7<>, i8sq7   )
FPM_SQ_BIND_LITERAL(  u8sqm4<>, u8sqm4  )
FPM_SQ_BIND_LITERAL(  u8sqm3<>, u8sqm3  )
FPM_SQ_BIND_LITERAL(  u8sqm2<>, u8sqm2  )
FPM_SQ_BIND_LITERAL(  u8sqm1<>, u8sqm1  )
FPM_SQ_BIND_LITERAL(   u8sq0<>, u8sq0   )
FPM_SQ_BIND_LITERAL(   u8sq1<>, u8sq1   )
FPM_SQ_BIND_LITERAL(   u8sq2<>, u8sq2   )
FPM_SQ_BIND_LITERAL(   u8sq3<>, u8sq3   )
FPM_SQ_BIND_LITERAL(   u8sq4<>, u8sq4   )
FPM_SQ_BIND_LITERAL(   u8sq5<>, u8sq5   )
FPM_SQ_BIND_LITERAL(   u8sq6<>, u8sq6   )
FPM_SQ_BIND_LITERAL(   u8sq7<>, u8sq7   )
FPM_SQ_BIND_LITERAL( i16sqm8<>, i16sqm8 )
FPM_SQ_BIND_LITERAL( i16sqm7<>, i16sqm7 )
FPM_SQ_BIND_LITERAL( i16sqm6<>, i16sqm6 )
FPM_SQ_BIND_LITERAL( i16sqm5<>, i16sqm5 )
FPM_SQ_BIND_LITERAL( i16sqm4<>, i16sqm4 )
FPM_SQ_BIND_LITERAL( i16sqm3<>, i16sqm3 )
FPM_SQ_BIND_LITERAL( i16sqm2<>, i16sqm2 )
FPM_SQ_BIND_LITERAL( i16sqm1<>, i16sqm1 )
FPM_SQ_BIND_LITERAL(  i16sq0<>, i16sq0  )
FPM_SQ_BIND_LITERAL(  i16sq1<>, i16sq1  )
FPM_SQ_BIND_LITERAL(  i16sq2<>, i16sq2  )
FPM_SQ_BIND_LITERAL(  i16sq3<>, i16sq3  )
FPM_SQ_BIND_LITERAL(  i16sq4<>, i16sq4  )
FPM_SQ_BIND_LITERAL(  i16sq5<>, i16sq5  )
FPM_SQ_BIND_LITERAL(  i16sq6<>, i16sq6  )
FPM_SQ_BIND_LITERAL(  i16sq7<>, i16sq7  )
FPM_SQ_BIND_LITERAL(  i16sq8<>, i16sq8  )
FPM_SQ_BIND_LITERAL(  i16sq9<>, i16sq9  )
FPM_SQ_BIND_LITERAL( i16sq10<>, i16sq10 )
FPM_SQ_BIND_LITERAL( i16sq11<>, i16sq11 )
FPM_SQ_BIND_LITERAL( i16sq12<>, i16sq12 )
FPM_SQ_BIND_LITERAL( i16sq13<>, i16sq13 )
FPM_SQ_BIND_LITERAL( i16sq14<>, i16sq14 )
FPM_SQ_BIND_LITERAL( i16sq15<>, i16sq15 )
FPM_SQ_BIND_LITERAL( u16sqm8<>, u16sqm8 )
FPM_SQ_BIND_LITERAL( u16sqm7<>, u16sqm7 )
FPM_SQ_BIND_LITERAL( u16sqm6<>, u16sqm6 )
FPM_SQ_BIND_LITERAL( u16sqm5<>, u16sqm5 )
FPM_SQ_BIND_LITERAL( u16sqm4<>, u16sqm4 )
FPM_SQ_BIND_LITERAL( u16sqm3<>, u16sqm3 )
FPM_SQ_BIND_LITERAL( u16sqm2<>, u16sqm2 )
FPM_SQ_BIND_LITERAL( u16sqm1<>, u16sqm1 )
FPM_SQ_BIND_LITERAL(  u16sq0<>, u16sq0  )
FPM_SQ_BIND_LITERAL(  u16sq1<>, u16sq1  )
FPM_SQ_BIND_LITERAL(  u16sq2<>, u16sq2  )
FPM_SQ_BIND_LITERAL(  u16sq3<>, u16sq3  )
FPM_SQ_BIND_LITERAL(  u16sq4<>, u16sq4  )
FPM_SQ_BIND_LITERAL(  u16sq5<>, u16sq5  )
FPM_SQ_BIND_LITERAL(  u16sq6<>, u16sq6  )
FPM_SQ_BIND_LITERAL(  u16sq7<>, u16sq7  )
FPM_SQ_BIND_LITERAL(  u16sq8<>, u16sq8  )
FPM_SQ_BIND_LITERAL(  u16sq9<>, u16sq9  )
FPM_SQ_BIND_LITERAL( u16sq10<>, u16sq10 )
FPM_SQ_BIND_LITERAL( u16sq11<>, u16sq11 )
FPM_SQ_BIND_LITERAL( u16sq12<>, u16sq12 )
FPM_SQ_BIND_LITERAL( u16sq13<>, u16sq13 )
FPM_SQ_BIND_LITERAL( u16sq14<>, u16sq14 )
FPM_SQ_BIND_LITERAL( u16sq15<>, u16sq15 )
FPM_SQ_BIND_LITERAL(i32sqm16<>, i32sqm16)
FPM_SQ_BIND_LITERAL(i32sqm15<>, i32sqm15)
FPM_SQ_BIND_LITERAL(i32sqm14<>, i32sqm14)
FPM_SQ_BIND_LITERAL(i32sqm13<>, i32sqm13)
FPM_SQ_BIND_LITERAL(i32sqm12<>, i32sqm12)
FPM_SQ_BIND_LITERAL(i32sqm11<>, i32sqm11)
FPM_SQ_BIND_LITERAL(i32sqm10<>, i32sqm10)
FPM_SQ_BIND_LITERAL( i32sqm9<>, i32sqm9 )
FPM_SQ_BIND_LITERAL( i32sqm8<>, i32sqm8 )
FPM_SQ_BIND_LITERAL( i32sqm7<>, i32sqm7 )
FPM_SQ_BIND_LITERAL( i32sqm6<>, i32sqm6 )
FPM_SQ_BIND_LITERAL( i32sqm5<>, i32sqm5 )
FPM_SQ_BIND_LITERAL( i32sqm4<>, i32sqm4 )
FPM_SQ_BIND_LITERAL( i32sqm3<>, i32sqm3 )
FPM_SQ_BIND_LITERAL( i32sqm2<>, i32sqm2 )
FPM_SQ_BIND_LITERAL( i32sqm1<>, i32sqm1 )
FPM_SQ_BIND_LITERAL(  i32sq0<>, i32sq0  )
FPM_SQ_BIND_LITERAL(  i32sq1<>, i32sq1  )
FPM_SQ_BIND_LITERAL(  i32sq2<>, i32sq2  )
FPM_SQ_BIND_LITERAL(  i32sq3<>, i32sq3  )
FPM_SQ_BIND_LITERAL(  i32sq4<>, i32sq4  )
FPM_SQ_BIND_LITERAL(  i32sq5<>, i32sq5  )
FPM_SQ_BIND_LITERAL(  i32sq6<>, i32sq6  )
FPM_SQ_BIND_LITERAL(  i32sq7<>, i32sq7  )
FPM_SQ_BIND_LITERAL(  i32sq8<>, i32sq8  )
FPM_SQ_BIND_LITERAL(  i32sq9<>, i32sq9  )
FPM_SQ_BIND_LITERAL( i32sq10<>, i32sq10 )
FPM_SQ_BIND_LITERAL( i32sq11<>, i32sq11 )
FPM_SQ_BIND_LITERAL( i32sq12<>, i32sq12 )
FPM_SQ_BIND_LITERAL( i32sq13<>, i32sq13 )
FPM_SQ_BIND_LITERAL( i32sq14<>, i32sq14 )
FPM_SQ_BIND_LITERAL( i32sq15<>, i32sq15 )
FPM_SQ_BIND_LITERAL( i32sq16<>, i32sq16 )
FPM_SQ_BIND_LITERAL( i32sq17<>, i32sq17 )
FPM_SQ_BIND_LITERAL( i32sq18<>, i32sq18 )
FPM_SQ_BIND_LITERAL( i32sq19<>, i32sq19 )
FPM_SQ_BIND_LITERAL( i32sq20<>, i32sq20 )
FPM_SQ_BIND_LITERAL( i32sq21<>, i32sq21 )
FPM_SQ_BIND_LITERAL( i32sq22<>, i32sq22 )
FPM_SQ_BIND_LITERAL( i32sq23<>, i32sq23 )
FPM_SQ_BIND_LITERAL( i32sq24<>, i32sq24 )
FPM_SQ_BIND_LITERAL( i32sq25<>, i32sq25 )
FPM_SQ_BIND_LITERAL( i32sq26<>, i32sq26 )
FPM_SQ_BIND_LITERAL( i32sq27<>, i32sq27 )
FPM_SQ_BIND_LITERAL( i32sq28<>, i32sq28 )
FPM_SQ_BIND_LITERAL( i32sq29<>, i32sq29 )
FPM_SQ_BIND_LITERAL( i32sq30<>, i32sq30 )
FPM_SQ_BIND_LITERAL( i32sq31<>, i32sq31 )
FPM_SQ_BIND_LITERAL(u32sqm16<>, u32sqm16)
FPM_SQ_BIND_LITERAL(u32sqm15<>, u32sqm15)
FPM_SQ_BIND_LITERAL(u32sqm14<>, u32sqm14)
FPM_SQ_BIND_LITERAL(u32sqm13<>, u32sqm13)
FPM_SQ_BIND_LITERAL(u32sqm12<>, u32sqm12)
FPM_SQ_BIND_LITERAL(u32sqm11<>, u32sqm11)
FPM_SQ_BIND_LITERAL(u32sqm10<>, u32sqm10)
FPM_SQ_BIND_LITERAL( u32sqm9<>, u32sqm9 )
FPM_SQ_BIND_LITERAL( u32sqm8<>, u32sqm8 )
FPM_SQ_BIND_LITERAL( u32sqm7<>, u32sqm7 )
FPM_SQ_BIND_LITERAL( u32sqm6<>, u32sqm6 )
FPM_SQ_BIND_LITERAL( u32sqm5<>, u32sqm5 )
FPM_SQ_BIND_LITERAL( u32sqm4<>, u32sqm4 )
FPM_SQ_BIND_LITERAL( u32sqm3<>, u32sqm3 )
FPM_SQ_BIND_LITERAL( u32sqm2<>, u32sqm2 )
FPM_SQ_BIND_LITERAL( u32sqm1<>, u32sqm1 )
FPM_SQ_BIND_LITERAL(  u32sq0<>, u32sq0  )
FPM_SQ_BIND_LITERAL(  u32sq1<>, u32sq1  )
FPM_SQ_BIND_LITERAL(  u32sq2<>, u32sq2  )
FPM_SQ_BIND_LITERAL(  u32sq3<>, u32sq3  )
FPM_SQ_BIND_LITERAL(  u32sq4<>, u32sq4  )
FPM_SQ_BIND_LITERAL(  u32sq5<>, u32sq5  )
FPM_SQ_BIND_LITERAL(  u32sq6<>, u32sq6  )
FPM_SQ_BIND_LITERAL(  u32sq7<>, u32sq7  )
FPM_SQ_BIND_LITERAL(  u32sq8<>, u32sq8  )
FPM_SQ_BIND_LITERAL(  u32sq9<>, u32sq9  )
FPM_SQ_BIND_LITERAL( u32sq10<>, u32sq10 )
FPM_SQ_BIND_LITERAL( u32sq11<>, u32sq11 )
FPM_SQ_BIND_LITERAL( u32sq12<>, u32sq12 )
FPM_SQ_BIND_LITERAL( u32sq13<>, u32sq13 )
FPM_SQ_BIND_LITERAL( u32sq14<>, u32sq14 )
FPM_SQ_BIND_LITERAL( u32sq15<>, u32sq15 )
FPM_SQ_BIND_LITERAL( u32sq16<>, u32sq16 )
FPM_SQ_BIND_LITERAL( u32sq17<>, u32sq17 )
FPM_SQ_BIND_LITERAL( u32sq18<>, u32sq18 )
FPM_SQ_BIND_LITERAL( u32sq19<>, u32sq19 )
FPM_SQ_BIND_LITERAL( u32sq20<>, u32sq20 )
FPM_SQ_BIND_LITERAL( u32sq21<>, u32sq21 )
FPM_SQ_BIND_LITERAL( u32sq22<>, u32sq22 )
FPM_SQ_BIND_LITERAL( u32sq23<>, u32sq23 )
FPM_SQ_BIND_LITERAL( u32sq24<>, u32sq24 )
FPM_SQ_BIND_LITERAL( u32sq25<>, u32sq25 )
FPM_SQ_BIND_LITERAL( u32sq26<>, u32sq26 )
FPM_SQ_BIND_LITERAL( u32sq27<>, u32sq27 )
FPM_SQ_BIND_LITERAL( u32sq28<>, u32sq28 )
FPM_SQ_BIND_LITERAL( u32sq29<>, u32sq29 )
FPM_SQ_BIND_LITERAL( u32sq30<>, u32sq30 )
FPM_SQ_BIND_LITERAL( u32sq31<>, u32sq31 )

/**\}*/
}

#endif
// EOF
