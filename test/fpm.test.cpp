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

TEST_F(InternalTest, abs__signed_positive__returns_unsigned_positive) {
    auto result = detail::abs((int)+36);

    ASSERT_TRUE((std::is_same_v<int, decltype(result)>));
    ASSERT_EQ(+36u, result);
}

TEST_F(InternalTest, abs__signed_negative__returns_unsigned_positive) {
    auto result = detail::abs((int)-36);

    ASSERT_TRUE((std::is_same_v<int, decltype(result)>));
    ASSERT_EQ(+36u, result);
}

TEST_F(InternalTest, abs__zero__returns_zero) {
    auto result = detail::abs((int)0);

    ASSERT_TRUE((std::is_same_v<int, decltype(result)>));
    ASSERT_EQ(0u, result);
}

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
    auto result = s2smd<double,from,to>(-564.1);

    constexpr double expectedResult = -72204.8;
    ASSERT_NEAR(expectedResult, result, 0.1);
}

TEST_F(S2STest, s2smd__constexpr_double_positiveF__double_negativeF) {
    constexpr scaling_t from = 4, to = -2;
    auto result = s2smd<double,from,to>(-29876.1);

    constexpr double expectedResult = -466.8;
    ASSERT_NEAR(expectedResult, result, 0.1);
}

TEST_F(S2STest, s2smd__constexpr_double_sameF__sameF_same_double) {
    constexpr scaling_t from = -2, to = -2;
    constexpr double real = -564.1;
    auto result = s2smd<double,from,to>(real);

    ASSERT_NEAR(real, result, 0.1);
}

TEST_F(S2STest, s2smd__var_signed_negativeF__int_positiveF) {
    constexpr scaling_t from = -2, to = 5;
    auto result = s2smd<int,from,to>(a);  // generates a function call when runtime variable is used

    constexpr int expectedResult = 512;
    ASSERT_EQ(expectedResult, result);
}

TEST_F(S2STest, s2sh__var_signed_negativeF__int_positiveF) {
    constexpr scaling_t from = -2, to = 5;
    auto result = s2sh<int,from,to>(a);  // generates a function call when runtime variable is used

    constexpr int expectedResult = 512;
    ASSERT_EQ(expectedResult, result);
}

TEST_F(S2STest, s2smd__var_signed_positiveF__int_largerF) {
    constexpr scaling_t from = 2, to = 7;
    auto result = s2smd<int,from,to>(a);

    constexpr int expectedResult = 128;
    ASSERT_EQ(expectedResult, result);
}

TEST_F(S2STest, s2sh__var_signed_positiveF__int_largerF) {
    constexpr scaling_t from = 2, to = 7;
    auto result = s2sh<int,from,to>(a);

    constexpr int expectedResult = 128;
    ASSERT_EQ(expectedResult, result);
}

TEST_F(S2STest, s2smd__var_signed_positiveF__int_smallerF) {
    constexpr scaling_t from = 6, to = 2;
    auto result = s2smd<int,from,to>(b);

    constexpr int expectedResult = -32;
    ASSERT_EQ(expectedResult, result);
}

TEST_F(S2STest, s2sh__var_signed_positiveF__int_smallerF) {
    constexpr scaling_t from = 6, to = 2;
    auto result = s2sh<int,from,to>(b);

    constexpr int expectedResult = -32;
    ASSERT_EQ(expectedResult, result);
}

TEST_F(S2STest, s2smd__var_signed_positiveF__int_negativeF) {
    constexpr scaling_t from = 4, to = -2;
    auto result = s2smd<int,from,to>(b);

    constexpr int expectedResult = -8;
    ASSERT_EQ(expectedResult, result);
}

TEST_F(S2STest, s2sh__var_signed_positiveF__int_negativeF) {
    constexpr scaling_t from = 4, to = -2;
    auto result = s2sh<int,from,to>(b);

    constexpr int expectedResult = -8;
    ASSERT_EQ(expectedResult, result);
}

TEST_F(S2STest, s2smd__var_signed_positiveF__int_sameF) {
    constexpr scaling_t from = 4, to = 4;
    auto result = s2smd<int,from,to>(b);

    ASSERT_EQ(b, result);
}

TEST_F(S2STest, s2sh__var_signed_positiveF__int_sameF) {
    constexpr scaling_t from = 4, to = 4;
    auto result = s2sh<int,from,to>(b);

    ASSERT_EQ(b, result);
}

TEST_F(S2STest, s2smd__var_signed_positiveF__double_zeroF) {
    constexpr scaling_t from = 4, to = 0;
    auto result = s2smd<double,from,to>(c);

    constexpr double expectedResult = -34.1875;
    ASSERT_NEAR(expectedResult, result, 0.0625);  // precision = 2^-4 = 1/16 = 0.0625
}

TEST_F(S2STest, s2smd__constexpr_signed_negativeF__int_positiveF) {
    constexpr scaling_t from = -4, to = 5;
    constexpr int8_t value = 4;
    auto result = s2smd<int,from,to>(value);  // expands to a compile-time const expr. when possible

    constexpr int expectedResult = 2048;
    ASSERT_EQ(expectedResult, result);
}

TEST_F(S2STest, s2sh__constexpr_signed_negativeF__int_positiveF) {
    constexpr scaling_t from = -4, to = 5;
    constexpr int8_t value = 4;
    auto result = s2sh<int,from,to>(value);  // expands to a compile-time const expr. when possible

    constexpr int expectedResult = 2048;
    ASSERT_EQ(expectedResult, result);
}

TEST_F(S2STest, s2smd__constexpr_signed_positiveF__int_largerF) {
    constexpr scaling_t from = 3, to = 7;
    constexpr int8_t value = 4;
    auto result = s2smd<int,from,to>(value);

    constexpr int expectedResult = 64;
    ASSERT_EQ(expectedResult, result);
}

TEST_F(S2STest, s2sh__constexpr_signed_positiveF__int_largerF) {
    constexpr scaling_t from = 3, to = 7;
    constexpr int8_t value = 4;
    auto result = s2sh<int,from,to>(value);

    constexpr int expectedResult = 64;
    ASSERT_EQ(expectedResult, result);
}

TEST_F(S2STest, s2smd__constexpr_signed_positiveF__int_smallerF) {
    constexpr scaling_t from = 9, to = 2;
    constexpr int16_t value = -512;
    auto result = s2smd<int,from,to>(value);

    constexpr int expectedResult = -4;
    ASSERT_EQ(expectedResult, result);
}

TEST_F(S2STest, s2sh__constexpr_signed_positiveF__int_smallerF) {
    constexpr scaling_t from = 9, to = 2;
    constexpr int16_t value = -512;
    auto result = s2sh<int,from,to>(value);

    constexpr int expectedResult = -4;
    ASSERT_EQ(expectedResult, result);
}

TEST_F(S2STest, s2smd__constexpr_signed_positiveF__int_negativeF) {
    constexpr scaling_t from = 3, to = -3;
    constexpr int16_t value = -512;
    auto result = s2smd<int,from,to>(value);

    constexpr int expectedResult = -8;
    ASSERT_EQ(expectedResult, result);
}

TEST_F(S2STest, s2sh__constexpr_signed_positiveF__int_negativeF) {
    constexpr scaling_t from = 3, to = -3;
    constexpr int16_t value = -512;
    auto result = s2sh<int,from,to>(value);

    constexpr int expectedResult = -8;
    ASSERT_EQ(expectedResult, result);
}

TEST_F(S2STest, s2smd__constexpr_signed_positiveF__int_sameF) {
    constexpr scaling_t from = 0, to = 0;
    constexpr int16_t value = -512;
    auto result = s2smd<int,from,to>(value);

    ASSERT_EQ(value, result);
}

TEST_F(S2STest, s2sh__constexpr_signed_positiveF__int_sameF) {
    constexpr scaling_t from = 0, to = 0;
    constexpr int16_t value = -512;
    auto result = s2sh<int,from,to>(value);

    ASSERT_EQ(value, result);
}

TEST_F(S2STest, s2smd__constexpr_signed_positiveF__double_zeroF) {
    constexpr scaling_t from = 6, to = 0;
    constexpr int16_t value = 547;
    auto result = s2smd<double,from,to>(value);

    constexpr double expectedResult = 8.546875;
    ASSERT_NEAR(expectedResult, result, 0.016);  // precision = 2^-6 = 1/64 ~ 0.016
}

TEST_F(S2STest, s2smd__constexpr_signed__int_symmetric_output) {
    constexpr scaling_t from = 8, to = 4;
    constexpr int16_t value = 514;
    auto resultP = s2smd<int,from,to>(+value);
    auto resultN = s2smd<int,from,to>(-value);

    constexpr int expectedResult = 32;
    ASSERT_EQ(+expectedResult, resultP);
    ASSERT_EQ(-expectedResult, resultN);
}

TEST_F(S2STest, s2sh__constexpr_signed__int_asymmetric_output) {
    constexpr scaling_t from = 8, to = 4;
    constexpr int16_t value = 514;
    auto resultP = s2sh<int,from,to>(+value);
    auto resultN = s2sh<int,from,to>(-value);

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

TEST_F(V2STest, v2s__constexpr_double_positiveF__scaled_int) {
    constexpr scaling_t to = 5;
    constexpr double real = 4.8971;
    auto result = v2s<int,to>(real);

    constexpr int expectedResult = 156;
    ASSERT_EQ(expectedResult, result);
}

TEST_F(V2STest, v2s__constexpr_double_negativeF__scaled_int) {
    constexpr scaling_t to = -4;
    constexpr double real = 4897.1;
    auto result = v2s<int,to>(real);

    constexpr int expectedResult = 306;
    ASSERT_EQ(expectedResult, result);
}

TEST_F(V2STest, v2s__constexpr_double_zeroF__scaled_int) {
    constexpr scaling_t to = 0;
    constexpr double real = 4897.1;
    auto result = v2s<int,to>(real);

    constexpr int expectedResult = 4897;
    ASSERT_EQ(expectedResult, result);
}

TEST_F(V2STest, v2s__constexpr_pos_and_neg_double__symmetric_output) {
    constexpr scaling_t to = 4;
    constexpr double real = 36.9999;
    auto resultP = v2s<int,to>(+real);
    auto resultN = v2s<int,to>(-real);

    constexpr int expectedResult = 591;
    ASSERT_EQ(+expectedResult, resultP);
    ASSERT_EQ(-expectedResult, resultN);
}

TEST_F(V2STest, v2s__constexpr_pos_and_neg_double__double_output) {
    constexpr scaling_t to = 4;
    constexpr double real = 16.66;
    auto resultP = v2s<double,to>(+real);
    auto resultN = v2s<double,to>(-real);

    constexpr double expectedResult = 266.56;
    ASSERT_NEAR(+expectedResult, resultP, 0.0625);  // precision = 2^-4 = 1/16 = 0.0625
    ASSERT_NEAR(-expectedResult, resultN, 0.0625);
}

TEST_F(V2STest, v2s__large_F__double_output) {
    constexpr double realMin = std::numeric_limits<int16_t>().min();
    constexpr double realMax = std::numeric_limits<int16_t>().max();
    auto resultN = v2s<double,MAX_F-std::numeric_limits<int16_t>::digits>(realMin);  // f: 53-15=38
    auto resultP = v2s<double,MAX_F-std::numeric_limits<int16_t>::digits>(realMax);

    constexpr double expectedResultN = -9.007199254740992e15;  // epsilon is very close to 1.0 for this number
    constexpr double expectedResultP = +9.006924376834048e15;
    ASSERT_NEAR(expectedResultN, resultN, 1.0);
    ASSERT_NEAR(expectedResultP, resultP, 1.0);
}


// EOF
