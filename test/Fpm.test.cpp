/* \file
 * Tests for fpm.hpp.
 */

#include <gtest/gtest.h>

#include <iostream>
using namespace std;

#include <fpm/sq.hpp>


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
    using sq_base_t = int16_t;
    using unwrap_t = int;
    auto sqValue = fpm::sq<sq_base_t, 4, -2048., 2047.9>(-2047.6);
    EXPECT_TRUE(( std::is_same< sq_base_t, decltype(sqValue.Reveal()) >::value ));
    EXPECT_TRUE(( std::is_same< unwrap_t, decltype(sqValue.Unwrap<unwrap_t>()) >::value ));
    ASSERT_EQ(-32761, sqValue.Reveal());
    ASSERT_EQ(-2048, sqValue.Unwrap<int>());
}

template< typename BASE_T, fpm::scaling_t F, double V_MIN, double V_MAX, double V >
concept CanConstructSq = requires {
    fpm::sq<BASE_T, F, V_MIN, V_MAX>(V);
};
TEST_F(SQTest, sq__constexpr_int16_F4_value_ooR__does_not_compile) {
    //static_assert(!CanConstructSq<int16_t, 4, -20., 20., 21.>);
    //static_assert(!CanConstructSq<int16_t, 4, -20., 20., -21.>);
    // TODO: implement this test: value out-of-range should not compile
}


// EOF