/* \file
 * Tests for fpm.hpp.
 */

#include <gtest/gtest.h>

#include <iostream>
using namespace std;

#include <fpm/sq.hpp>
#include <fpm/q.hpp>
using namespace fpm;


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

TEST_F(S2STest, s2s__var_signed_negativeF__int_positiveF) {
    constexpr scaling_t FROM = -2, TO = 5;
    auto result = s2s<int,FROM,TO>(a);  // generates a function call when runtime variable is used

    constexpr int RESULT = 512;
    ASSERT_EQ(RESULT, result);
}

TEST_F(S2STest, s2s__var_signed_positiveF__int_largerF) {
    constexpr scaling_t FROM = 2, TO = 7;
    auto result = s2s<int,FROM,TO>(a);

    constexpr int RESULT = 128;
    ASSERT_EQ(RESULT, result);
}

TEST_F(S2STest, s2s__var_signed_positiveF__int_smallerF) {
    constexpr scaling_t FROM = 6, TO = 2;
    auto result = s2s<int,FROM,TO>(b);

    constexpr int RESULT = -32;
    ASSERT_EQ(RESULT, result);
}

TEST_F(S2STest, s2s__var_signed_positiveF__int_negativeF) {
    constexpr scaling_t FROM = 4, TO = -2;
    auto result = s2s<int,FROM,TO>(b);

    constexpr int RESULT = -8;
    ASSERT_EQ(RESULT, result);
}

TEST_F(S2STest, s2s__var_signed_positiveF__int_sameF) {
    constexpr scaling_t FROM = 4, TO = 4;
    auto result = s2s<int,FROM,TO>(b);

    ASSERT_EQ(b, result);
}

TEST_F(S2STest, s2s__var_signed_positiveF__double_zeroF) {
    constexpr scaling_t FROM = 4, TO = 0;
    auto result = s2s<double,FROM,TO>(c);

    constexpr double RESULT = -34.1875;
    ASSERT_NEAR(RESULT, result, 0.0625);  // precision = 2^-4 = 1/16 = 0.0625
}

TEST_F(S2STest, s2s__constexpr_signed_negativeF__int_positiveF) {
    constexpr scaling_t FROM = -4, TO = 5;
    constexpr int8_t VALUE = 4;
    auto result = s2s<int,FROM,TO>(VALUE);  // expands to a compile-time const expr. when possible

    constexpr int RESULT = 2048;
    ASSERT_EQ(RESULT, result);
}

TEST_F(S2STest, s2s__constexpr_signed_positiveF__int_largerF) {
    constexpr scaling_t FROM = 3, TO = 7;
    constexpr int8_t VALUE = 4;
    auto result = s2s<int,FROM,TO>(VALUE);

    constexpr int RESULT = 64;
    ASSERT_EQ(RESULT, result);
}

TEST_F(S2STest, s2s__constexpr_signed_positiveF__int_smallerF) {
    constexpr scaling_t FROM = 9, TO = 2;
    constexpr int16_t VALUE = -512;
    auto result = s2s<int,FROM,TO>(VALUE);

    constexpr int RESULT = -4;
    ASSERT_EQ(RESULT, result);
}

TEST_F(S2STest, s2s__constexpr_signed_positiveF__int_negativeF) {
    constexpr scaling_t FROM = 3, TO = -3;
    constexpr int16_t VALUE = -512;
    auto result = s2s<int,FROM,TO>(VALUE);

    constexpr int RESULT = -8;
    ASSERT_EQ(RESULT, result);
}

TEST_F(S2STest, s2s__constexpr_signed_positiveF__int_sameF) {
    constexpr scaling_t FROM = 0, TO = 0;
    constexpr int16_t VALUE = -512;
    auto result = s2s<int,FROM,TO>(VALUE);

    ASSERT_EQ(VALUE, result);
}

TEST_F(S2STest, s2s__constexpr_signed_positiveF__double_zeroF) {
    constexpr scaling_t FROM = 6, TO = 0;
    constexpr int16_t VALUE = 547;
    auto result = s2s<double,FROM,TO>(VALUE);

    constexpr double RESULT = 8.546875;
    ASSERT_NEAR(RESULT, result, 0.016);  // precision = 2^-6 = 1/64 ~ 0.016
}

TEST_F(S2STest, s2s__constexpr_signed__int_symmetric_output) {
    constexpr scaling_t FROM = 8, TO = 4;
    constexpr int16_t VALUE = 514;
    auto resultP = s2s<int,FROM,TO>(+VALUE);
    auto resultN = s2s<int,FROM,TO>(-VALUE);

    constexpr int RESULT = 32;
    ASSERT_EQ(+RESULT, resultP);
    ASSERT_EQ(-RESULT, resultN);
}


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

    constexpr int RESULT = 156;
    ASSERT_EQ(RESULT, result);
}

TEST_F(V2STest, v2s__constexpr_double_negativeF__scaled_int) {
    constexpr scaling_t TO = -4;
    constexpr double REAL_VALUE = 4897.1;
    auto result = v2s<int,TO>(REAL_VALUE);

    constexpr int RESULT = 306;
    ASSERT_EQ(RESULT, result);
}

TEST_F(V2STest, v2s__constexpr_double_zeroF__scaled_int) {
    constexpr scaling_t TO = 0;
    constexpr double REAL_VALUE = 4897.1;
    auto result = v2s<int,TO>(REAL_VALUE);

    constexpr int RESULT = 4897;
    ASSERT_EQ(RESULT, result);
}

TEST_F(V2STest, v2s__constexpr_pos_and_neg_double__symmetric_output) {
    constexpr scaling_t TO = 4;
    constexpr double REAL_VALUE = 36.9999;
    auto resultP = v2s<int,TO>(+REAL_VALUE);
    auto resultN = v2s<int,TO>(-REAL_VALUE);

    constexpr int RESULT = 591;
    ASSERT_EQ(+RESULT, resultP);
    ASSERT_EQ(-RESULT, resultN);
}

TEST_F(V2STest, v2s__constexpr_pos_and_neg_double__double_output) {
    constexpr scaling_t TO = 4;
    constexpr double REAL_VALUE = 16.66;
    auto resultP = v2s<double,TO>(+REAL_VALUE);
    auto resultN = v2s<double,TO>(-REAL_VALUE);

    constexpr double RESULT = 266.56;
    ASSERT_NEAR(+RESULT, resultP, 0.0625);  // precision = 2^-4 = 1/16 = 0.0625
    ASSERT_NEAR(-RESULT, resultN, 0.0625);
}

TEST_F(V2STest, v2s__dynamic_construction__not_possible) {
    // TODO: use 'concept' and 'requires' to implement this check?
    //constexpr scaling_t TO = 2;
    //double a = -5897.9841;
    //v2s<int,TO>(a);  // this must not compile
}


class SQTest : public ::testing::Test {
protected:
    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

TEST_F(SQTest, sq__constexpr_int16_positiveF__reveal_scaled_int16_and_unwrap_int) {
    // note: from_real named constructor does not compile if value is out of range
    constexpr scaling_t F = 4;
    constexpr double REAL_VALUE = -2047.6;
    auto sqValue = sq<int16_t, F, -2048., 2047.9>::from_real<REAL_VALUE>();

    EXPECT_TRUE((std::is_same_v<int16_t, decltype(sqValue.reveal())>));
    EXPECT_TRUE((std::is_same_v<int, decltype(sqValue.unwrap<int>())>));
    EXPECT_TRUE((std::is_same_v<double, decltype(sqValue.unwrap<double>())>));

    constexpr int16_t RESULT_MEM_VALUE = -32761;
    ASSERT_EQ(RESULT_MEM_VALUE, sqValue.reveal());
    ASSERT_NEAR(REAL_VALUE, sqValue.unwrap<double>(), (v2s<double,-F>(1)));
}

TEST_F(SQTest, sq__constexpr_int16_F4_value_ooR__does_not_compile) {
    // TODO: are tests like this possible (e.g. with SFINAE, or with concepts)?
    //constexpr scaling_t F = 4;
    //static_assert(!CanConstructSq<int16_t, F, -20., 20., -20.1>);
    //static_assert(!CanConstructSq<int16_t, F, -20., 20., +20.1>);
}


class QTest : public ::testing::Test {
protected:
    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

TEST_F(QTest, q__constexpr_int16_positiveF__reveal_scaled_int16_and_unwrap_int) {
    // note: from_real named constructor does not compile if value is out of range
    constexpr scaling_t F = 6;
    constexpr double REAL_VALUE = -495.1;
    auto qValue = q<int16_t, F, -500., 500.>::from_real<REAL_VALUE>();

    EXPECT_TRUE((std::is_same_v<int16_t, decltype(qValue.reveal())>));
    EXPECT_TRUE((std::is_same_v<int, decltype(qValue.unwrap<int>())>));
    EXPECT_TRUE((std::is_same_v<double, decltype(qValue.unwrap<double>())>));

    constexpr int16_t RESULT_MEM_VALUE = -31686;
    ASSERT_EQ(RESULT_MEM_VALUE, qValue.reveal());
    ASSERT_NEAR(REAL_VALUE, qValue.unwrap<double>(), (v2s<double,-F>(1)));
}

TEST_F(QTest, q__variable_negative_int16_positiveF_saturate__reveal_scaled_int16_and_unwrap_int) {
    constexpr scaling_t F = 6;
    constexpr double REAL_VALUE = -511.;
    int16_t volatile value = v2s<int16_t,F>(-511.);
    auto qValue = q<int16_t, F, -500., 500., overflow::SATURATE>::construct(value);

    constexpr int16_t RESULT_MEM_VALUE = -32000;
    constexpr double SATURATED_REAL_VALUE = -500.;
    ASSERT_EQ(RESULT_MEM_VALUE, qValue.reveal());
    ASSERT_NEAR(SATURATED_REAL_VALUE, qValue.unwrap<double>(), (v2s<double,-F>(1)));
}

TEST_F(QTest, q__variable_positive_int16_positiveF_saturate__reveal_scaled_int16_and_unwrap_int) {
    constexpr scaling_t F = 6;
    constexpr double REAL_VALUE = +511.;
    int16_t volatile value = v2s<int16_t,F>(REAL_VALUE);
    auto qValue = q<int16_t, F, -500., 500., overflow::SATURATE>::construct(value);

    constexpr int16_t RESULT_MEM_VALUE = +32000;
    constexpr double SATURATED_REAL_VALUE = +500.;
    ASSERT_EQ(RESULT_MEM_VALUE, qValue.reveal());
    ASSERT_NEAR(SATURATED_REAL_VALUE, qValue.unwrap<double>(), (v2s<double,-F>(1)));
}

TEST_F(QTest, q__variable_negative_int16_positiveF_overflow_allowed__reveal_scaled_int16_and_unwrap_int) {
    constexpr scaling_t F = 6;
    constexpr double REAL_VALUE = -511.;
    int16_t volatile value = v2s<int16_t,F>(REAL_VALUE);
    auto qValue = q<int16_t, F, -500., 500., overflow::ALLOWED>::construct(value);

    constexpr int16_t RESULT_MEM_VALUE = -32704;
    ASSERT_EQ(RESULT_MEM_VALUE, qValue.reveal());
    ASSERT_NEAR(REAL_VALUE, qValue.unwrap<double>(), (v2s<double,-F>(1)));
}

TEST_F(QTest, q__variable_positive_int16_positiveF_overflow_allowed__reveal_scaled_int16_and_unwrap_int) {
    constexpr scaling_t F = 6;
    constexpr double REAL_VALUE = +511.;
    int16_t volatile value = v2s<int16_t,F>(REAL_VALUE);
    auto qValue = q<int16_t, F, -500., 500., overflow::ALLOWED>::construct(value);

    constexpr int16_t RESULT_MEM_VALUE = +32704;
    ASSERT_EQ(RESULT_MEM_VALUE, qValue.reveal());
    ASSERT_NEAR(REAL_VALUE, qValue.unwrap<double>(), (v2s<double,-F>(1)));
}


// EOF