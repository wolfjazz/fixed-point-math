/* \file
 * Main header. Include this header to use the fpm library.
 */

#ifndef FPM_HPP_
#define FPM_HPP_

#include "fpm/fpm.hpp"
#include "fpm/sq.hpp"
#include "fpm/q.hpp"

// predefined types and literals
#include "fpm/sq_types.hpp"
#include "fpm/q_types.hpp"

/// Namespace providing all predefined types.
namespace fpm::types {
    using namespace fpm::q::types;
    using namespace fpm::sq::types;
    using fpm::detail::operator ""_ic;
}

#endif
// EOF
