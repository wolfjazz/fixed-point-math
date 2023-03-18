/* \file
 * Tests for fpm.hpp.
 */

#include <gtest/gtest.h>

#include <iostream>
using namespace std;

#include <fpm/sq.hpp>
#include <fpm/q.hpp>


class S2STest : public ::testing::Test
{
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
    auto result = fpm::s2s<int,-2,5>(a);  // generates a function call when runtime variable is used
    ASSERT_EQ(512, result);
}

TEST_F(S2STest, s2s__var_signed_positiveF__int_largerF) {
    auto result = fpm::s2s<int,2,7>(a);
    ASSERT_EQ(128, result);
}

TEST_F(S2STest, s2s__var_signed_positiveF__int_smallerF) {
    auto result = fpm::s2s<int,6,2>(b);
    ASSERT_EQ(-32, result);
}

TEST_F(S2STest, s2s__var_signed_positiveF__int_negativeF) {
    auto result = fpm::s2s<int,4,-2>(b);
    ASSERT_EQ(-8, result);
}

TEST_F(S2STest, s2s__var_signed_positiveF__int_sameF) {
    auto result = fpm::s2s<int,4,4>(b);
    ASSERT_EQ(b, result);
}

TEST_F(S2STest, s2s__constexpr_signed_negativeF__int_positiveF) {
    auto result = fpm::s2s<int,-4,5>((int8_t)4);  // expands to a compile-time const expr. when possible
    ASSERT_EQ(2048, result);
}

TEST_F(S2STest, s2s__constexpr_signed_positiveF__int_largerF) {
    auto result = fpm::s2s<int,3,7>((int8_t)4);
    ASSERT_EQ(64, result);
}

TEST_F(S2STest, s2s__constexpr_signed_positiveF__int_smallerF) {
    auto result = fpm::s2s<int,9,2>((int16_t)-512);
    ASSERT_EQ(-4, result);
}

TEST_F(S2STest, s2s__constexpr_signed_positiveF__int_negativeF) {
    auto result = fpm::s2s<int,3,-3>((int16_t)-512);
    ASSERT_EQ(-8, result);
}

TEST_F(S2STest, s2s__constexpr_signed_positiveF__int_sameF) {
    auto result = fpm::s2s<int,0,0>((int16_t)-512);
    ASSERT_EQ(-512, result);
}

TEST_F(S2STest, s2s__constexpr_signed__int_symmetric_output) {
    auto resultP = fpm::s2s<int,8,4>((int16_t)+514);
    auto resultN = fpm::s2s<int,8,4>((int16_t)-514);
    ASSERT_EQ(+32, resultP);
    ASSERT_EQ(-32, resultN);
}


class V2STest : public ::testing::Test
{
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
    auto result = fpm::v2s<int, 5>(4.8971);
    ASSERT_EQ(156, result);
}

TEST_F(V2STest, v2s__constexpr_double_negativeF__scaled_int) {
    auto result = fpm::v2s<int, -4>(4897.1);
    ASSERT_EQ(306, result);
}

TEST_F(V2STest, v2s__constexpr_double_zeroF__scaled_int) {
    auto result = fpm::v2s<int, 0>(4897.1);
    ASSERT_EQ(4897, result);
}

TEST_F(V2STest, v2s__constexpr_pos_and_neg_double__symmetric_output) {
    auto resultP = fpm::v2s<int, 4>(36.9999);
    auto resultN = fpm::v2s<int, 4>(-36.9999);
    ASSERT_EQ(+591, resultP);
    ASSERT_EQ(-591, resultN);
}

TEST_F(V2STest, v2s__dynamic_construction__not_possible) {
    // TODO: use 'concept' and 'requires' to implement this check?
    //fpm::v2s<int, 2>(a);  // this must not compile
}


class SQTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F(SQTest, sq__constexpr_int16_positiveF__reveal_scaled_int16_and_unwrap_int) {
    auto sqValue = fpm::sq<int16_t, 4, -2048., 2047.9>::from_real<-2047.6>();
    EXPECT_TRUE(( std::is_same< int16_t, decltype(sqValue.reveal()) >::value ));
    EXPECT_TRUE(( std::is_same< int, decltype(sqValue.unwrap<int>()) >::value ));
    ASSERT_EQ(-32761, sqValue.reveal());
    ASSERT_EQ(-2047, sqValue.unwrap<int>());
}

TEST_F(SQTest, sq__constexpr_int16_F4_value_ooR__does_not_compile) {
    // TODO: are tests like this possible (e.g. with SFINAE, or with concepts)?
    //static_assert(!CanConstructSq<int16_t, 4, -20., 20., -20.1>);
    //static_assert(!CanConstructSq<int16_t, 4, -20., 20., +20.1>);
}


class QTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F(QTest, q__constexpr_int16_positiveF__reveal_scaled_int16_and_unwrap_int) {
    auto qValue = fpm::q<int16_t, 6, -500., 500.>::from_real<-495.1>();
    EXPECT_TRUE(( std::is_same< int16_t, decltype(qValue.reveal()) >::value ));
    EXPECT_TRUE(( std::is_same< int, decltype(qValue.unwrap<int>()) >::value ));
    ASSERT_EQ(-31686, qValue.reveal());
    ASSERT_EQ(-495, qValue.unwrap<int>());
}


// EOF