/* \file
 * Tests for fpm.hpp.
 */

#include <gtest/gtest.h>

#include <iostream>
using namespace std;

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
};

TEST_F(InternalTest, abs__signed_positive__returns_unsigned_positive) {
    auto result = details::abs((int)+36);

    ASSERT_TRUE((std::is_same_v<unsigned int, decltype(result)>));
    ASSERT_EQ(+36u, result);
}

TEST_F(InternalTest, abs__signed_negative__returns_unsigned_positive) {
    auto result = details::abs((int)-36);

    ASSERT_TRUE((std::is_same_v<unsigned int, decltype(result)>));
    ASSERT_EQ(+36u, result);
}

TEST_F(InternalTest, abs__signed_negative_min__returns_unsigned_positive) {
    auto result = details::abs(std::numeric_limits<int>::min());

    ASSERT_TRUE((std::is_same_v<unsigned int, decltype(result)>));
    ASSERT_EQ((unsigned)std::numeric_limits<int>::max() + 1u, result);
}

TEST_F(InternalTest, min__signed_negative__returns_smaller_value) {
    auto result = details::min((int)-23, -45);
    ASSERT_EQ((int)-45, result);
}

TEST_F(InternalTest, min__signed_positive__returns_smaller_value) {
    auto result = details::min((int)89, 1145);
    ASSERT_EQ((int)89, result);
}

TEST_F(InternalTest, min__signed_mixed__returns_smaller_value) {
    auto result = details::min((int)2299, -1166);
    ASSERT_EQ((int)-1166, result);
}

TEST_F(InternalTest, max__signed_negative__returns_larger_value) {
    auto result = details::max((int)-6, -45);
    ASSERT_EQ((int)-6, result);
}

TEST_F(InternalTest, max__signed_positive__returns_larger_value) {
    auto result = details::max((int)189, 9145);
    ASSERT_EQ((int)9145, result);
}

TEST_F(InternalTest, max__signed_mixed__returns_larger_value) {
    auto result = details::max((int)3366, -5879);
    ASSERT_EQ((int)3366, result);
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
    constexpr double realValue = -564.1;
    auto result = s2smd<double,from,to>(realValue);

    ASSERT_NEAR(realValue, result, 0.1);
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
    constexpr double realValue = 4.8971;
    auto result = v2s<int,to>(realValue);

    constexpr int expectedResult = 156;
    ASSERT_EQ(expectedResult, result);
}

TEST_F(V2STest, v2s__constexpr_double_negativeF__scaled_int) {
    constexpr scaling_t to = -4;
    constexpr double realValue = 4897.1;
    auto result = v2s<int,to>(realValue);

    constexpr int expectedResult = 306;
    ASSERT_EQ(expectedResult, result);
}

TEST_F(V2STest, v2s__constexpr_double_zeroF__scaled_int) {
    constexpr scaling_t to = 0;
    constexpr double realValue = 4897.1;
    auto result = v2s<int,to>(realValue);

    constexpr int expectedResult = 4897;
    ASSERT_EQ(expectedResult, result);
}

TEST_F(V2STest, v2s__constexpr_pos_and_neg_double__symmetric_output) {
    constexpr scaling_t to = 4;
    constexpr double realValue = 36.9999;
    auto resultP = v2s<int,to>(+realValue);
    auto resultN = v2s<int,to>(-realValue);

    constexpr int expectedResult = 591;
    ASSERT_EQ(+expectedResult, resultP);
    ASSERT_EQ(-expectedResult, resultN);
}

TEST_F(V2STest, v2s__constexpr_pos_and_neg_double__double_output) {
    constexpr scaling_t to = 4;
    constexpr double realValue = 16.66;
    auto resultP = v2s<double,to>(+realValue);
    auto resultN = v2s<double,to>(-realValue);

    constexpr double expectedResult = 266.56;
    ASSERT_NEAR(+expectedResult, resultP, 0.0625);  // precision = 2^-4 = 1/16 = 0.0625
    ASSERT_NEAR(-expectedResult, resultN, 0.0625);
}

TEST_F(V2STest, v2s__large_F__double_output) {
    constexpr double realValueMin = std::numeric_limits<int16_t>().min();
    constexpr double realValueMax = std::numeric_limits<int16_t>().max();
    auto resultN = v2s<double,MAX_F-std::numeric_limits<int16_t>::digits>(realValueMin);  // f: 53-15=38
    auto resultP = v2s<double,MAX_F-std::numeric_limits<int16_t>::digits>(realValueMax);

    constexpr double expectedResultN = -9.007199254740992e15;
    constexpr double expectedResultP = +9.006924376834048e15;
    EXPECT_NEAR(details::test::floatpEpsilonFor(expectedResultN), 1.0, 1.0e-10);
    EXPECT_NEAR(details::test::floatpEpsilonFor(expectedResultP), 1.0, 1.0e-10);
    ASSERT_NEAR(expectedResultN, resultN, details::test::floatpEpsilonFor(expectedResultN));
    ASSERT_NEAR(expectedResultP, resultP, details::test::floatpEpsilonFor(expectedResultP));
}


// EOF
