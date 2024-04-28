/* \file
 * Tests for fpm.hpp.
 */

#include <gtest/gtest.h>

#include <iostream>

#include <fpm.hpp>
using namespace fpm;

// ////////////////////////////////////////////////////////////////////////////////////////////// //
// -------------------------------- Internal Functions Test ------------------------------------- //
// ////////////////////////////////////////////////////////////////////////////////////////////// //

class InternalTest : public ::testing::Test {
protected:
    void SetUp() override
    {
    }
    void TearDown() override
    {
    }

    /** \returns the minimum distance between doubles (epsilon) for numbers of the magnitude
     * of the given value.
     * \warning Expensive when used in production code! */
    inline double floatpEpsilonFor(double value) noexcept {
        double epsilon = nextafter(value, std::numeric_limits<double>::infinity()) - value;
        return epsilon;
    }
};

TEST_F(InternalTest, doubleFromLiteral__int__returns_double) {
    auto result = detail::doubleFromLiteral<'1', '2', '3'>();

    constexpr double expectedResult = 123.;
    ASSERT_NEAR(expectedResult, result, floatpEpsilonFor(expectedResult));
}

TEST_F(InternalTest, doubleFromLiteral__large_int__returns_double) {
    auto result = detail::doubleFromLiteral<'9', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1', '2', '3', '4', '5'>();

    constexpr double expectedResult = 923456789012345.;
    ASSERT_NEAR(expectedResult, result, floatpEpsilonFor(expectedResult));
}

TEST_F(InternalTest, doubleFromLiteral__zero__returns_double) {
    auto result = detail::doubleFromLiteral<'0'>();

    constexpr double expectedResult = 0.;
    ASSERT_NEAR(expectedResult, result, floatpEpsilonFor(expectedResult));
}

TEST_F(InternalTest, doubleFromLiteral__double__returns_double) {
    auto result = detail::doubleFromLiteral<'1', '2', '.', '3', '4', '5', '6'>();

    constexpr double expectedResult = 12.3456;
    ASSERT_NEAR(expectedResult, result, floatpEpsilonFor(expectedResult));
}

TEST_F(InternalTest, doubleFromLiteral__precise_double__returns_double) {
    auto result = detail::doubleFromLiteral<'1', '.', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1', '2', '3', '4'>();

    constexpr double expectedResult = 1.2345678901234;
    ASSERT_NEAR(expectedResult, result, floatpEpsilonFor(expectedResult));
}

TEST_F(InternalTest, doubleFromLiteral__double_with_exponent__returns_double) {
    auto resultS = detail::doubleFromLiteral<'1', '0', '0', '5', '.', '8', '9', '6', '7', 'e', '-', '2', '7'>();

    constexpr double expectedResult = 1005.8967e-27;
    ASSERT_NEAR(expectedResult, resultS, floatpEpsilonFor(expectedResult));
}

TEST_F(InternalTest, doubleFromLiteral__smallest_and_largest_double_with_exponent__returns_double) {
    auto resultS = detail::doubleFromLiteral<'2', '.', '2', '2', '5', '0', '7', 'e', '-', '3', '0', '8'>();
    auto resultL = detail::doubleFromLiteral<'1', '.', '7', '9', '7', '6', '9', 'e', '3', '0', '8'>();

    ASSERT_NEAR(2.22507e-308, resultS, 5e-323L);
    ASSERT_NEAR(1.79769e308, resultL, 4e292L);
}


// ////////////////////////////////////////////////////////////////////////////////////////////// //
// ------------------------------------ S2S Test ------------------------------------------------ //
// ////////////////////////////////////////////////////////////////////////////////////////////// //

class S2STest : public ::testing::Test {
protected:
    void SetUp() override
    {
    }
    void TearDown() override
    {
    }

    int8_t const a = 4;
    int16_t const b = -512;
    int16_t const c = -547;
};

TEST_F(S2STest, s2smd__constexpr_double_negativeF__double_positiveF) {
    constexpr scaling_t from = -2, to = 5;
    auto result = s2smd<from,to,double>(-564.1);

    constexpr double expectedResult = -72204.8;
    ASSERT_NEAR(expectedResult, result, 0.1);
}

TEST_F(S2STest, s2smd__constexpr_double_positiveF__double_negativeF) {
    constexpr scaling_t from = 4, to = -2;
    auto result = s2smd<from,to,double>(-29876.1);

    constexpr double expectedResult = -466.8;
    ASSERT_NEAR(expectedResult, result, 0.1);
}

TEST_F(S2STest, s2smd__constexpr_double_sameF__sameF_same_double) {
    constexpr scaling_t from = -2, to = -2;
    constexpr double real = -564.1;
    auto result = s2smd<from,to,double>(real);

    ASSERT_NEAR(real, result, 0.1);
}

TEST_F(S2STest, s2smd__var_signed_negativeF__int_positiveF) {
    constexpr scaling_t from = -2, to = 5;
    auto result = s2smd<from,to,int>(a);

    constexpr int expectedResult = 512;
    ASSERT_EQ(expectedResult, result);
}

TEST_F(S2STest, s2sh__var_signed_negativeF__int_positiveF) {
    constexpr scaling_t from = -2, to = 5;
    auto result = s2sh<from,to,int>(a);

    constexpr int expectedResult = 512;
    ASSERT_EQ(expectedResult, result);
}

TEST_F(S2STest, s2smd__var_signed_positiveF__int_largerF) {
    constexpr scaling_t from = 2, to = 7;
    auto result = s2smd<from,to,int>(a);

    constexpr int expectedResult = 128;
    ASSERT_EQ(expectedResult, result);
}

TEST_F(S2STest, s2sh__var_signed_positiveF__int_largerF) {
    constexpr scaling_t from = 2, to = 7;
    auto result = s2sh<from,to,int>(a);

    constexpr int expectedResult = 128;
    ASSERT_EQ(expectedResult, result);
}

TEST_F(S2STest, s2smd__var_signed_positiveF__int_smallerF) {
    constexpr scaling_t from = 6, to = 2;
    auto result = s2smd<from,to,int>(b);

    constexpr int expectedResult = -32;
    ASSERT_EQ(expectedResult, result);
}

TEST_F(S2STest, s2sh__var_signed_positiveF__int_smallerF) {
    constexpr scaling_t from = 6, to = 2;
    auto result = s2sh<from,to,int>(b);

    constexpr int expectedResult = -32;
    ASSERT_EQ(expectedResult, result);
}

TEST_F(S2STest, s2smd__var_signed_positiveF__int_negativeF) {
    constexpr scaling_t from = 4, to = -2;
    auto result = s2smd<from,to,int>(b);

    constexpr int expectedResult = -8;
    ASSERT_EQ(expectedResult, result);
}

TEST_F(S2STest, s2sh__var_signed_positiveF__int_negativeF) {
    constexpr scaling_t from = 4, to = -2;
    auto result = s2sh<from,to,int>(b);

    constexpr int expectedResult = -8;
    ASSERT_EQ(expectedResult, result);
}

TEST_F(S2STest, s2smd__var_signed_positiveF__int_sameF) {
    constexpr scaling_t from = 4, to = 4;
    auto result = s2smd<from,to,int>(b);

    ASSERT_EQ(b, result);
}

TEST_F(S2STest, s2sh__var_signed_positiveF__int_sameF) {
    constexpr scaling_t from = 4, to = 4;
    auto result = s2sh<from,to,int>(b);

    ASSERT_EQ(b, result);
}

TEST_F(S2STest, s2smd__var_signed_positiveF__double_zeroF) {
    constexpr scaling_t from = 4, to = 0;
    auto result = s2smd<from,to,double>(c);

    constexpr double expectedResult = -34.1875;
    ASSERT_NEAR(expectedResult, result, 0.0625);  // precision = 2^-4 = 1/16 = 0.0625
}

TEST_F(S2STest, s2smd__constexpr_signed_negativeF__int_positiveF) {
    constexpr scaling_t from = -4, to = 5;
    constexpr int8_t value = 4;
    auto result = s2smd<from,to,int>(value);

    constexpr int expectedResult = 2048;
    ASSERT_EQ(expectedResult, result);
}

TEST_F(S2STest, s2sh__constexpr_signed_negativeF__int_positiveF) {
    constexpr scaling_t from = -4, to = 5;
    constexpr int8_t value = 4;
    auto result = s2sh<from,to,int>(value);

    constexpr int expectedResult = 2048;
    ASSERT_EQ(expectedResult, result);
}

TEST_F(S2STest, s2smd__constexpr_signed_positiveF__int_largerF) {
    constexpr scaling_t from = 3, to = 7;
    constexpr int8_t value = 4;
    auto result = s2smd<from,to,int>(value);

    constexpr int expectedResult = 64;
    ASSERT_EQ(expectedResult, result);
}

TEST_F(S2STest, s2sh__constexpr_signed_positiveF__int_largerF) {
    constexpr scaling_t from = 3, to = 7;
    constexpr int8_t value = 4;
    auto result = s2sh<from,to,int>(value);

    constexpr int expectedResult = 64;
    ASSERT_EQ(expectedResult, result);
}

TEST_F(S2STest, s2smd__constexpr_signed_positiveF__int_smallerF) {
    constexpr scaling_t from = 9, to = 2;
    constexpr int16_t value = -512;
    auto result = s2smd<from,to,int>(value);

    constexpr int expectedResult = -4;
    ASSERT_EQ(expectedResult, result);
}

TEST_F(S2STest, s2sh__constexpr_signed_positiveF__int_smallerF) {
    constexpr scaling_t from = 9, to = 2;
    constexpr int16_t value = -512;
    auto result = s2sh<from,to,int>(value);

    constexpr int expectedResult = -4;
    ASSERT_EQ(expectedResult, result);
}

TEST_F(S2STest, s2smd__constexpr_signed_positiveF__int_negativeF) {
    constexpr scaling_t from = 3, to = -3;
    constexpr int16_t value = -512;
    auto result = s2smd<from,to,int>(value);

    constexpr int expectedResult = -8;
    ASSERT_EQ(expectedResult, result);
}

TEST_F(S2STest, s2sh__constexpr_signed_positiveF__int_negativeF) {
    constexpr scaling_t from = 3, to = -3;
    constexpr int16_t value = -512;
    auto result = s2sh<from,to,int>(value);

    constexpr int expectedResult = -8;
    ASSERT_EQ(expectedResult, result);
}

TEST_F(S2STest, s2smd__constexpr_signed_positiveF__int_sameF) {
    constexpr scaling_t from = 0, to = 0;
    constexpr int16_t value = -512;
    auto result = s2smd<from,to,int>(value);

    ASSERT_EQ(value, result);
}

TEST_F(S2STest, s2sh__constexpr_signed_positiveF__int_sameF) {
    constexpr scaling_t from = 0, to = 0;
    constexpr int16_t value = -512;
    auto result = s2sh<from,to,int>(value);

    ASSERT_EQ(value, result);
}

TEST_F(S2STest, s2smd__constexpr_small_signed_positiveF_min_value__larger_unsigned_smallerF) {
    constexpr scaling_t from = 4, to = -2;
    constexpr int16_t value = INT16_MIN;
    auto result = s2smd<from,to,uint32_t>(value);  // -32768 / 2^6 = -512

    constexpr uint32_t expectedResult = 4294966784;  // -512, interpreted as u32
    ASSERT_EQ(expectedResult, result);
}

TEST_F(S2STest, s2sh__constexpr_small_signed_positiveF_min_value__larger_unsigned_smallerF) {
    constexpr scaling_t from = 4, to = -2;
    constexpr int16_t value = INT16_MIN;
    auto result = s2sh<from,to,uint32_t>(value);  // -32768 / 2^6 = -512

    constexpr uint32_t expectedResult = 4294966784;  // -512, interpreted as u32
    ASSERT_EQ(expectedResult, result);
}

TEST_F(S2STest, s2smd__constexpr_small_unsigned_positiveF_max_value__larger_signed_smallerF) {
    constexpr scaling_t from = 4, to = -2;
    constexpr uint16_t value = UINT16_MAX;
    auto result = s2smd<from,to,int32_t>(value);

    constexpr int32_t expectedResult = 1023;
    ASSERT_EQ(expectedResult, result);
}

TEST_F(S2STest, s2sh__constexpr_small_unsigned_positiveF_max_value__larger_signed_smallerF) {
    constexpr scaling_t from = 4, to = -2;
    constexpr uint16_t value = UINT16_MAX;
    auto result = s2sh<from,to,int32_t>(value);

    constexpr int32_t expectedResult = 1023;
    ASSERT_EQ(expectedResult, result);
}

TEST_F(S2STest, s2smd__constexpr_signed_positiveF__double_zeroF) {
    constexpr scaling_t from = 6, to = 0;
    constexpr int16_t value = 547;
    auto result = s2smd<from,to,double>(value);

    constexpr double expectedResult = 8.546875;
    ASSERT_NEAR(expectedResult, result, 0.016);  // precision = 2^-6 = 1/64 ~ 0.016
}

TEST_F(S2STest, s2smd__constexpr_signed__int_symmetric_output) {
    constexpr scaling_t from = 8, to = 4;
    constexpr int16_t value = 514;
    auto resultP = s2smd<from,to,int>(+value);
    auto resultN = s2smd<from,to,int>(-value);

    constexpr int expectedResult = 32;
    ASSERT_EQ(+expectedResult, resultP);
    ASSERT_EQ(-expectedResult, resultN);
}

TEST_F(S2STest, s2sh__constexpr_signed__int_asymmetric_output) {
    constexpr scaling_t from = 8, to = 4;
    constexpr int16_t value = 514;
    auto resultP = s2sh<from,to,int>(+value);
    auto resultN = s2sh<from,to,int>(-value);

    constexpr int expectedResultP = +32;
    constexpr int expectedResultN = -33;
    ASSERT_EQ(expectedResultP, resultP);
    ASSERT_EQ(expectedResultN, resultN);
}


// ////////////////////////////////////////////////////////////////////////////////////////////// //
// ------------------------------------ V2S Test ------------------------------------------------ //
// ////////////////////////////////////////////////////////////////////////////////////////////// //

class V2STest : public ::testing::Test {
protected:
    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

TEST_F(V2STest, v2smd__constexpr_double_positiveF__double) {
    constexpr scaling_t to = 7;
    auto result = v2smd<to,double>(-564.1);

    constexpr double expectedResult = -72204.8;
    ASSERT_NEAR(expectedResult, result, 0.1);
}

TEST_F(V2STest, v2smd__constexpr_double_negativeF__double) {
    constexpr scaling_t to = -6;
    auto result = v2smd<to,double>(-29876.1);

    constexpr double expectedResult = -466.8;
    ASSERT_NEAR(expectedResult, result, 0.1);
}

TEST_F(V2STest, v2smd__constexpr_double_f0__same_double) {
    constexpr scaling_t to = 0;
    constexpr double real = -564.1;
    auto result = v2smd<to,double>(real);

    ASSERT_NEAR(real, result, 0.001);
}

TEST_F(V2STest, v2smd__constexpr_signed_positiveF__int) {
    constexpr scaling_t to = 9;
    constexpr int8_t value = 4;
    auto result = v2smd<to,int>(value);

    constexpr int expectedResult = 2048;
    ASSERT_EQ(expectedResult, result);
}

TEST_F(V2STest, v2sh__constexpr_signed_positiveF__int) {
    constexpr scaling_t to = 9;
    constexpr int8_t value = 4;
    auto result = v2sh<to,int>(value);

    constexpr int expectedResult = 2048;
    ASSERT_EQ(expectedResult, result);
}

TEST_F(V2STest, v2smd__constexpr_signed_negativeF__int) {
    constexpr scaling_t to = -6;
    constexpr int16_t value = -512;
    auto result = v2smd<to,int>(value);

    constexpr int expectedResult = -8;
    ASSERT_EQ(expectedResult, result);
}

TEST_F(V2STest, v2sh__constexpr_signed_negativeF__int) {
    constexpr scaling_t to = -6;
    constexpr int16_t value = -512;
    auto result = v2sh<to,int>(value);

    constexpr int expectedResult = -8;
    ASSERT_EQ(expectedResult, result);
}

TEST_F(V2STest, v2smd__constexpr_signed_f0__int) {
    constexpr scaling_t to = 0;
    constexpr int16_t value = -512;
    auto result = v2smd<to,int>(value);

    ASSERT_EQ(value, result);
}

TEST_F(V2STest, v2sh__constexpr_signed_f0__int) {
    constexpr scaling_t to = 0;
    constexpr int16_t value = -512;
    auto result = v2sh<to,int>(value);

    ASSERT_EQ(value, result);
}

TEST_F(V2STest, v2smd__constexpr_small_signed_min_value_negativeF__larger_unsigned) {
    constexpr scaling_t to = -6;
    constexpr int16_t value = INT16_MIN;
    auto result = v2smd<to,uint32_t>(value);  // -32768 / 2^6 = -512

    constexpr uint32_t expectedResult = 4294966784;  // -512, interpreted as u32
    ASSERT_EQ(expectedResult, result);
}

TEST_F(V2STest, v2sh__constexpr_small_signed_min_value_negativeF__larger_unsigned) {
    constexpr scaling_t to = -6;
    constexpr int16_t value = INT16_MIN;
    auto result = v2sh<to,uint32_t>(value);  // -32768 / 2^6 = -512

    constexpr uint32_t expectedResult = 4294966784;  // -512, interpreted as u32
    ASSERT_EQ(expectedResult, result);
}

TEST_F(V2STest, v2smd__constexpr_small_unsigned_max_value_negativeF__larger_signed) {
    constexpr scaling_t to = -6;
    constexpr uint16_t value = UINT16_MAX;
    auto result = v2smd<to,int32_t>(value);

    constexpr int32_t expectedResult = 1023;
    ASSERT_EQ(expectedResult, result);
}

TEST_F(V2STest, v2sh__constexpr_small_unsigned_max_value_negativeF__larger_signed) {
    constexpr scaling_t to = -6;
    constexpr uint16_t value = UINT16_MAX;
    auto result = v2sh<to,int32_t>(value);

    constexpr int32_t expectedResult = 1023;
    ASSERT_EQ(expectedResult, result);
}

TEST_F(V2STest, v2smd__constexpr_signed_negativeF__int_symmetric_output) {
    constexpr scaling_t to = -4;
    constexpr int16_t value = 514;
    auto resultP = v2smd<to,int>(+value);
    auto resultN = v2smd<to,int>(-value);

    constexpr int expectedResult = 32;
    ASSERT_EQ(+expectedResult, resultP);
    ASSERT_EQ(-expectedResult, resultN);
}

TEST_F(V2STest, v2sh__constexpr_signed_negativeF__int_asymmetric_output) {
    constexpr scaling_t to = -4;
    constexpr int16_t value = 514;
    auto resultP = v2sh<to,int>(+value);
    auto resultN = v2sh<to,int>(-value);

    constexpr int expectedResultP = +32;
    constexpr int expectedResultN = -33;
    ASSERT_EQ(expectedResultP, resultP);
    ASSERT_EQ(expectedResultN, resultN);
}

TEST_F(V2STest, v2smd__constexpr_large_F__double_output) {
    constexpr double realMin = std::numeric_limits<int16_t>().min();
    constexpr double realMax = std::numeric_limits<int16_t>().max();
    auto resultN = v2smd<detail::MAX_F-std::numeric_limits<int16_t>::digits,double>(realMin);  // f: 53-15=38
    auto resultP = v2smd<detail::MAX_F-std::numeric_limits<int16_t>::digits,double>(realMax);

    constexpr double expectedResultN = -9.007199254740992e15;  // epsilon is very close to 1.0 for this number
    constexpr double expectedResultP = +9.006924376834048e15;
    ASSERT_NEAR(expectedResultN, resultN, 1.0);
    ASSERT_NEAR(expectedResultP, resultP, 1.0);
}


// EOF
