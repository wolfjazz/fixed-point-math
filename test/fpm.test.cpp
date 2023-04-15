/* \file
 * Tests for fpm.hpp.
 */

#include <gtest/gtest.h>

#include <iostream>
using namespace std;

#include <fpm/fpm.hpp>
using namespace fpm;


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

TEST_F(S2STest, s2s__constexpr_double_negativeF__double_positiveF) {
    constexpr scaling_t FROM = -2, TO = 5;
    auto result = s2s<double,FROM,TO>(-564.1);

    constexpr double EXPECTED_RESULT = -72204.8;
    ASSERT_NEAR(EXPECTED_RESULT, result, 0.1);
}

TEST_F(S2STest, s2s__constexpr_double_positiveF__double_negativeF) {
    constexpr scaling_t FROM = 4, TO = -2;
    auto result = s2s<double,FROM,TO>(-29876.1);

    constexpr double EXPECTED_RESULT = -466.8;
    ASSERT_NEAR(EXPECTED_RESULT, result, 0.1);
}

TEST_F(S2STest, s2s__constexpr_double_sameF__sameF_same_double) {
    constexpr scaling_t FROM = -2, TO = -2;
    constexpr double REAL_VALUE = -564.1;
    auto result = s2s<double,FROM,TO>(REAL_VALUE);

    ASSERT_NEAR(REAL_VALUE, result, 0.1);
}

TEST_F(S2STest, s2s__var_signed_negativeF__int_positiveF) {
    constexpr scaling_t FROM = -2, TO = 5;
    auto result = s2s<int,FROM,TO>(a);  // generates a function call when runtime variable is used

    constexpr int EXPECTED_RESULT = 512;
    ASSERT_EQ(EXPECTED_RESULT, result);
}

TEST_F(S2STest, s2sh__var_signed_negativeF__int_positiveF) {
    constexpr scaling_t FROM = -2, TO = 5;
    auto result = s2sh<int,FROM,TO>(a);  // generates a function call when runtime variable is used

    constexpr int EXPECTED_RESULT = 512;
    ASSERT_EQ(EXPECTED_RESULT, result);
}

TEST_F(S2STest, s2s__var_signed_positiveF__int_largerF) {
    constexpr scaling_t FROM = 2, TO = 7;
    auto result = s2s<int,FROM,TO>(a);

    constexpr int EXPECTED_RESULT = 128;
    ASSERT_EQ(EXPECTED_RESULT, result);
}

TEST_F(S2STest, s2sh__var_signed_positiveF__int_largerF) {
    constexpr scaling_t FROM = 2, TO = 7;
    auto result = s2sh<int,FROM,TO>(a);

    constexpr int EXPECTED_RESULT = 128;
    ASSERT_EQ(EXPECTED_RESULT, result);
}

TEST_F(S2STest, s2s__var_signed_positiveF__int_smallerF) {
    constexpr scaling_t FROM = 6, TO = 2;
    auto result = s2s<int,FROM,TO>(b);

    constexpr int EXPECTED_RESULT = -32;
    ASSERT_EQ(EXPECTED_RESULT, result);
}

TEST_F(S2STest, s2sh__var_signed_positiveF__int_smallerF) {
    constexpr scaling_t FROM = 6, TO = 2;
    auto result = s2sh<int,FROM,TO>(b);

    constexpr int EXPECTED_RESULT = -32;
    ASSERT_EQ(EXPECTED_RESULT, result);
}

TEST_F(S2STest, s2s__var_signed_positiveF__int_negativeF) {
    constexpr scaling_t FROM = 4, TO = -2;
    auto result = s2s<int,FROM,TO>(b);

    constexpr int EXPECTED_RESULT = -8;
    ASSERT_EQ(EXPECTED_RESULT, result);
}

TEST_F(S2STest, s2sh__var_signed_positiveF__int_negativeF) {
    constexpr scaling_t FROM = 4, TO = -2;
    auto result = s2sh<int,FROM,TO>(b);

    constexpr int EXPECTED_RESULT = -8;
    ASSERT_EQ(EXPECTED_RESULT, result);
}

TEST_F(S2STest, s2s__var_signed_positiveF__int_sameF) {
    constexpr scaling_t FROM = 4, TO = 4;
    auto result = s2s<int,FROM,TO>(b);

    ASSERT_EQ(b, result);
}

TEST_F(S2STest, s2sh__var_signed_positiveF__int_sameF) {
    constexpr scaling_t FROM = 4, TO = 4;
    auto result = s2sh<int,FROM,TO>(b);

    ASSERT_EQ(b, result);
}

TEST_F(S2STest, s2s__var_signed_positiveF__double_zeroF) {
    constexpr scaling_t FROM = 4, TO = 0;
    auto result = s2s<double,FROM,TO>(c);

    constexpr double EXPECTED_RESULT = -34.1875;
    ASSERT_NEAR(EXPECTED_RESULT, result, 0.0625);  // precision = 2^-4 = 1/16 = 0.0625
}

TEST_F(S2STest, s2s__constexpr_signed_negativeF__int_positiveF) {
    constexpr scaling_t FROM = -4, TO = 5;
    constexpr int8_t VALUE = 4;
    auto result = s2s<int,FROM,TO>(VALUE);  // expands to a compile-time const expr. when possible

    constexpr int EXPECTED_RESULT = 2048;
    ASSERT_EQ(EXPECTED_RESULT, result);
}

TEST_F(S2STest, s2sh__constexpr_signed_negativeF__int_positiveF) {
    constexpr scaling_t FROM = -4, TO = 5;
    constexpr int8_t VALUE = 4;
    auto result = s2sh<int,FROM,TO>(VALUE);  // expands to a compile-time const expr. when possible

    constexpr int EXPECTED_RESULT = 2048;
    ASSERT_EQ(EXPECTED_RESULT, result);
}

TEST_F(S2STest, s2s__constexpr_signed_positiveF__int_largerF) {
    constexpr scaling_t FROM = 3, TO = 7;
    constexpr int8_t VALUE = 4;
    auto result = s2s<int,FROM,TO>(VALUE);

    constexpr int EXPECTED_RESULT = 64;
    ASSERT_EQ(EXPECTED_RESULT, result);
}

TEST_F(S2STest, s2sh__constexpr_signed_positiveF__int_largerF) {
    constexpr scaling_t FROM = 3, TO = 7;
    constexpr int8_t VALUE = 4;
    auto result = s2sh<int,FROM,TO>(VALUE);

    constexpr int EXPECTED_RESULT = 64;
    ASSERT_EQ(EXPECTED_RESULT, result);
}

TEST_F(S2STest, s2s__constexpr_signed_positiveF__int_smallerF) {
    constexpr scaling_t FROM = 9, TO = 2;
    constexpr int16_t VALUE = -512;
    auto result = s2s<int,FROM,TO>(VALUE);

    constexpr int EXPECTED_RESULT = -4;
    ASSERT_EQ(EXPECTED_RESULT, result);
}

TEST_F(S2STest, s2sh__constexpr_signed_positiveF__int_smallerF) {
    constexpr scaling_t FROM = 9, TO = 2;
    constexpr int16_t VALUE = -512;
    auto result = s2sh<int,FROM,TO>(VALUE);

    constexpr int EXPECTED_RESULT = -4;
    ASSERT_EQ(EXPECTED_RESULT, result);
}

TEST_F(S2STest, s2s__constexpr_signed_positiveF__int_negativeF) {
    constexpr scaling_t FROM = 3, TO = -3;
    constexpr int16_t VALUE = -512;
    auto result = s2s<int,FROM,TO>(VALUE);

    constexpr int EXPECTED_RESULT = -8;
    ASSERT_EQ(EXPECTED_RESULT, result);
}

TEST_F(S2STest, s2sh__constexpr_signed_positiveF__int_negativeF) {
    constexpr scaling_t FROM = 3, TO = -3;
    constexpr int16_t VALUE = -512;
    auto result = s2sh<int,FROM,TO>(VALUE);

    constexpr int EXPECTED_RESULT = -8;
    ASSERT_EQ(EXPECTED_RESULT, result);
}

TEST_F(S2STest, s2s__constexpr_signed_positiveF__int_sameF) {
    constexpr scaling_t FROM = 0, TO = 0;
    constexpr int16_t VALUE = -512;
    auto result = s2s<int,FROM,TO>(VALUE);

    ASSERT_EQ(VALUE, result);
}

TEST_F(S2STest, s2sh__constexpr_signed_positiveF__int_sameF) {
    constexpr scaling_t FROM = 0, TO = 0;
    constexpr int16_t VALUE = -512;
    auto result = s2sh<int,FROM,TO>(VALUE);

    ASSERT_EQ(VALUE, result);
}

TEST_F(S2STest, s2s__constexpr_signed_positiveF__double_zeroF) {
    constexpr scaling_t FROM = 6, TO = 0;
    constexpr int16_t VALUE = 547;
    auto result = s2s<double,FROM,TO>(VALUE);

    constexpr double EXPECTED_RESULT = 8.546875;
    ASSERT_NEAR(EXPECTED_RESULT, result, 0.016);  // precision = 2^-6 = 1/64 ~ 0.016
}

TEST_F(S2STest, s2s__constexpr_signed__int_symmetric_output) {
    constexpr scaling_t FROM = 8, TO = 4;
    constexpr int16_t VALUE = 514;
    auto resultP = s2s<int,FROM,TO>(+VALUE);
    auto resultN = s2s<int,FROM,TO>(-VALUE);

    constexpr int EXPECTED_RESULT = 32;
    ASSERT_EQ(+EXPECTED_RESULT, resultP);
    ASSERT_EQ(-EXPECTED_RESULT, resultN);
}

TEST_F(S2STest, s2sh__constexpr_signed__int_asymmetric_output) {
    constexpr scaling_t FROM = 8, TO = 4;
    constexpr int16_t VALUE = 514;
    auto resultP = s2sh<int,FROM,TO>(+VALUE);
    auto resultN = s2sh<int,FROM,TO>(-VALUE);

    constexpr int EXPECTED_RESULT_P = +32;
    constexpr int EXPECTED_RESULT_N = -33;
    ASSERT_EQ(EXPECTED_RESULT_P, resultP);
    ASSERT_EQ(EXPECTED_RESULT_N, resultN);
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
    constexpr scaling_t TO = 5;
    constexpr double REAL_VALUE = 4.8971;
    auto result = v2s<int,TO>(REAL_VALUE);

    constexpr int EXPECTED_RESULT = 156;
    ASSERT_EQ(EXPECTED_RESULT, result);
}

TEST_F(V2STest, v2s__constexpr_double_negativeF__scaled_int) {
    constexpr scaling_t TO = -4;
    constexpr double REAL_VALUE = 4897.1;
    auto result = v2s<int,TO>(REAL_VALUE);

    constexpr int EXPECTED_RESULT = 306;
    ASSERT_EQ(EXPECTED_RESULT, result);
}

TEST_F(V2STest, v2s__constexpr_double_zeroF__scaled_int) {
    constexpr scaling_t TO = 0;
    constexpr double REAL_VALUE = 4897.1;
    auto result = v2s<int,TO>(REAL_VALUE);

    constexpr int EXPECTED_RESULT = 4897;
    ASSERT_EQ(EXPECTED_RESULT, result);
}

TEST_F(V2STest, v2s__constexpr_pos_and_neg_double__symmetric_output) {
    constexpr scaling_t TO = 4;
    constexpr double REAL_VALUE = 36.9999;
    auto resultP = v2s<int,TO>(+REAL_VALUE);
    auto resultN = v2s<int,TO>(-REAL_VALUE);

    constexpr int EXPECTED_RESULT = 591;
    ASSERT_EQ(+EXPECTED_RESULT, resultP);
    ASSERT_EQ(-EXPECTED_RESULT, resultN);
}

TEST_F(V2STest, v2s__constexpr_pos_and_neg_double__double_output) {
    constexpr scaling_t TO = 4;
    constexpr double REAL_VALUE = 16.66;
    auto resultP = v2s<double,TO>(+REAL_VALUE);
    auto resultN = v2s<double,TO>(-REAL_VALUE);

    constexpr double EXPECTED_RESULT = 266.56;
    ASSERT_NEAR(+EXPECTED_RESULT, resultP, 0.0625);  // precision = 2^-4 = 1/16 = 0.0625
    ASSERT_NEAR(-EXPECTED_RESULT, resultN, 0.0625);
}

TEST_F(V2STest, v2s__dynamic_construction__not_possible) {
    // TODO: use 'concept' and 'requires' or SFINAE to implement this check?
    //constexpr scaling_t TO = 2;
    //double a = -5897.9841;
    //v2s<int,TO>(a);  // this must not compile
}


// EOF
