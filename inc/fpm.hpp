/* \file
 * Main header. Include this header to use the fpm library.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
}  // namespace types
}  // namespace fpm

#endif
// EOF
