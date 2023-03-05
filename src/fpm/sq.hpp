/** \file
 * Declaration of the sq class template.
 */

#ifndef _FPM_SQ_HPP_
#define _FPM_SQ_HPP_

#include "fpm.hpp"


namespace fpm {

template< typename BASE_T, scaling_t F, double V_MIN, double V_MAX, overflow OVF_ACTION = overflow::FORBIDDEN >
class sq final
{
    static_assert(std::is_integral<BASE_T>::value, "base type must be integral");

public:
    static constexpr BASE_T T_MIN = numeric_limits<BASE_T>::min();  ///< minimum possible value of base type
    static constexpr BASE_T T_MAX = numeric_limits<BASE_T>::max();  ///< maximum possible value of base type
    static constexpr BASE_T MIN = v2s<BASE_T, F>(V_MIN);  ///< minimum value of integer value range
    static constexpr BASE_T MAX = v2s<BASE_T, F>(V_MAX);  ///< maximum value of integer value range

    /// Explicit compile-time-only constructor from floating-point value.
    explicit constexpr sq(double value) : sq(v2s<BASE_T, F>(value))
    {
        //constexpr int64_t intermediate = v2s<BASE_T, F>(value);
        //static_assert(intermediate >= T_MIN && intermediate <= T_MAX, "value is out of range");
        //sq(static_cast<BASE_T>(intermediate));
    }

    /// Destructor.
    ~sq()
    {}

    /// Reveals the integer value stored in the memory.
    BASE_T Reveal() const {
        return value;
    }

    /// Unwraps the unscaled value.
    /// \warning Unscaled value will be cast to integer type (no floats at runtime).
    BASE_T Unwrap() const {
        return s2s<F,0>(value);
    }

private:
    // delete default constructor
    sq() = delete;

    /// Runtime constructor from integer value.
    explicit sq(BASE_T value) : value(value)
    {
        // TODO: assert that value is within valid range
    }

    /// scaled integer value that represents a fixed-point value; stored in memory
    BASE_T value;
};

}


#endif
// EOF
