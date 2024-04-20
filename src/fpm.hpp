/* \file
 * Main header. Include this header to use the fpm library.
 */

#ifndef FPM_HPP_F76CC86E_BBCD_46BC_A775_6D44D82C72C3
#define FPM_HPP_F76CC86E_BBCD_46BC_A775_6D44D82C72C3

#include "fpm/fpm.hpp"
#include "fpm/sq.hpp"
#include "fpm/q.hpp"

// predefined types and literals
#include "fpm/sq_types.hpp"
#include "fpm/q_types.hpp"

/// Namespace providing all predefined types.
namespace fpm {
using q::Q;
using sq::Sq;

namespace types {
using namespace fpm::q::types;
using namespace fpm::sq::types;
using fpm::operator ""_ic;
using fpm::operator-;
}
}

#endif
// EOF
