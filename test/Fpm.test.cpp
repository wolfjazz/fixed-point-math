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
};

TEST_F(S2STest, s2s__var_signed_negativeF__int_positiveF) {
    auto result = s2s<int,-2,5>(a);  // generates a function call when runtime variable is used
    ASSERT_EQ(512, result);
}

TEST_F(S2STest, s2s__var_signed_positiveF__int_largerF) {
    auto result = s2s<int,2,7>(a);
    ASSERT_EQ(128, result);
}

TEST_F(S2STest, s2s__var_signed_positiveF__int_smallerF) {
    auto result = s2s<int,6,2>(b);
    ASSERT_EQ(-32, result);
}

TEST_F(S2STest, s2s__var_signed_positiveF__int_negativeF) {
    auto result = s2s<int,4,-2>(b);
    ASSERT_EQ(-8, result);
}

TEST_F(S2STest, s2s__var_signed_positiveF__int_sameF) {
    auto result = s2s<int,4,4>(b);
    ASSERT_EQ(b, result);
}

TEST_F(S2STest, s2s__constexpr_signed_negativeF__int_positiveF) {
    auto result = s2s<int,-4,5>((int8_t)4);  // expands to a compile-time const expr. when possible
    ASSERT_EQ(2048, result);
}

TEST_F(S2STest, s2s__constexpr_signed_positiveF__int_largerF) {
    auto result = s2s<int,3,7>((int8_t)4);
    ASSERT_EQ(64, result);
}

TEST_F(S2STest, s2s__constexpr_signed_positiveF__int_smallerF) {
    auto result = s2s<int,9,2>((int16_t)-512);
    ASSERT_EQ(-4, result);
}

TEST_F(S2STest, s2s__constexpr_signed_positiveF__int_negativeF) {
    auto result = s2s<int,3,-3>((int16_t)-512);
    ASSERT_EQ(-8, result);
}

TEST_F(S2STest, s2s__constexpr_signed_positiveF__int_sameF) {
    auto result = s2s<int,0,0>((int16_t)-512);
    ASSERT_EQ(-512, result);
}

TEST_F(S2STest, s2s__constexpr_signed__int_symmetric_output) {
    auto resultP = s2s<int,8,4>((int16_t)+514);
    auto resultN = s2s<int,8,4>((int16_t)-514);
    ASSERT_EQ(+32, resultP);
    ASSERT_EQ(-32, resultN);
}


class V2STest : public ::testing::Test {
protected:
    void SetUp() override
    {
    }
    void TearDown() override
    {
    }

    double const a = -5897.9841;
};

TEST_F(V2STest, v2s__constexpr_double_positiveF__scaled_int) {
    auto result = v2s<int,5>(4.8971);
    ASSERT_EQ(156, result);
}

TEST_F(V2STest, v2s__constexpr_double_negativeF__scaled_int) {
    auto result = v2s<int,-4>(4897.1);
    ASSERT_EQ(306, result);
}

TEST_F(V2STest, v2s__constexpr_double_zeroF__scaled_int) {
    auto result = v2s<int,0>(4897.1);
    ASSERT_EQ(4897, result);
}

TEST_F(V2STest, v2s__constexpr_pos_and_neg_double__symmetric_output) {
    auto resultP = v2s<int,4>(36.9999);
    auto resultN = v2s<int,4>(-36.9999);
    ASSERT_EQ(+591, resultP);
    ASSERT_EQ(-591, resultN);
}

TEST_F(V2STest, v2s__dynamic_construction__not_possible) {
    // TODO: use 'concept' and 'requires' to implement this check?
    //v2s<int, 2>(a);  // this must not compile
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
    auto sqValue = sq<int16_t, 4, -2048., 2047.9>::from_real<-2047.6>();
    EXPECT_TRUE((std::is_same_v<int16_t, decltype(sqValue.reveal())>));
    EXPECT_TRUE((std::is_same_v<int, decltype(sqValue.unwrap<int>())>));
    ASSERT_EQ(-32761, sqValue.reveal());
    ASSERT_EQ(-2047, sqValue.unwrap<int>());
}

TEST_F(SQTest, sq__constexpr_int16_F4_value_ooR__does_not_compile) {
    // TODO: are tests like this possible (e.g. with SFINAE, or with concepts)?
    //static_assert(!CanConstructSq<int16_t, 4, -20., 20., -20.1>);
    //static_assert(!CanConstructSq<int16_t, 4, -20., 20., +20.1>);
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
    // note: from_real named constructors do not compile if value is out of range
    auto qValue = q<int16_t, 6, -500., 500.>::from_real<-495.1>();
    EXPECT_TRUE((std::is_same_v<int16_t, decltype(qValue.reveal())>));
    EXPECT_TRUE((std::is_same_v<int, decltype(qValue.unwrap<int>())>));
    ASSERT_EQ(-31686, qValue.reveal());
    ASSERT_EQ(-495, qValue.unwrap<int>());
}

TEST_F(QTest, q__variable_negative_int16_positiveF_saturate__reveal_scaled_int16_and_unwrap_int) {
    int16_t volatile value = v2s<int16_t,6>(-511.);
    auto qValue = q<int16_t, 6, -500., 500., overflow::SATURATE>::construct(value);
    EXPECT_TRUE((std::is_same_v<int16_t, decltype(qValue.reveal())>));
    EXPECT_TRUE((std::is_same_v<int, decltype(qValue.unwrap<int>())>));
    ASSERT_EQ(-32000, qValue.reveal());
    ASSERT_EQ(-500, qValue.unwrap<int>());
}

TEST_F(QTest, q__variable_positive_int16_positiveF_saturate__reveal_scaled_int16_and_unwrap_int) {
    int16_t volatile value = v2s<int16_t,6>(+511.);
    auto qValue = q<int16_t, 6, -500., 500., overflow::SATURATE>::construct(value);
    EXPECT_TRUE((std::is_same_v<int16_t, decltype(qValue.reveal())>));
    EXPECT_TRUE((std::is_same_v<int, decltype(qValue.unwrap<int>())>));
    ASSERT_EQ(+32000, qValue.reveal());
    ASSERT_EQ(+500, qValue.unwrap<int>());
}

TEST_F(QTest, q__variable_negative_int16_positiveF_overflow_allowed__reveal_scaled_int16_and_unwrap_int) {
    int16_t volatile value = v2s<int16_t,6>(-511.);
    auto qValue = q<int16_t, 6, -500., 500., overflow::ALLOWED>::construct(value);
    EXPECT_TRUE((std::is_same_v<int16_t, decltype(qValue.reveal())>));
    EXPECT_TRUE((std::is_same_v<int, decltype(qValue.unwrap<int>())>));
    ASSERT_EQ(-32704, qValue.reveal());
    ASSERT_EQ(-511, qValue.unwrap<int>());
}

TEST_F(QTest, q__variable_positive_int16_positiveF_overflow_allowed__reveal_scaled_int16_and_unwrap_int) {
    int16_t volatile value = v2s<int16_t,6>(+511.);
    auto qValue = q<int16_t, 6, -500., 500., overflow::ALLOWED>::construct(value);
    EXPECT_TRUE((std::is_same_v<int16_t, decltype(qValue.reveal())>));
    EXPECT_TRUE((std::is_same_v<int, decltype(qValue.unwrap<int>())>));
    ASSERT_EQ(+32704, qValue.reveal());
    ASSERT_EQ(+511, qValue.unwrap<int>());
}


// EOF