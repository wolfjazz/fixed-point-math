/* \file
 * Tests for sq.hpp.
 */

#include <gtest/gtest.h>

#include <iostream>
using namespace std;

#include <fpm/sq.hpp>
#include <fpm/q.hpp>
using namespace fpm;


// ////////////////////////////////////////////////////////////////////////////////////////////// //
// ------------------------------------ SQ Test: Construction ----------------------------------- //
// ////////////////////////////////////////////////////////////////////////////////////////////// //

class SQTest_Construct : public ::testing::Test {
protected:
    using i16sq4 = sq<int16_t, 4, -2048., 2047.9>;
    using i32sq4 = sq<int32_t, 4, -2048., 2048.>;
    using i32sqm2 = sq<int32_t, -2, -2048., 2048.>;
    using i32sq8 = sq<int32_t, 8, -2048.1, 2048.1>;

    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

TEST_F(SQTest_Construct, sq_delimit__some_sq_type__delimited_sq_type) {
    constexpr double RESTRICTED_LIMIT = 1024.;
    constexpr double EXTENDED_LIMIT = 4096.;
    using restricted_sq_t   = i32sq4::delimit<-RESTRICTED_LIMIT,  +RESTRICTED_LIMIT>;
    using restricted_l_sq_t = i32sq4::delimit<-RESTRICTED_LIMIT,  i32sq4::REAL_V_MAX>;
    using restricted_r_sq_t = i32sq4::delimit<i32sq4::REAL_V_MIN, +RESTRICTED_LIMIT>;
    using extended_sq_t     = i32sq4::delimit<-EXTENDED_LIMIT,    +EXTENDED_LIMIT>;
    using extended_l_sq_t   = i32sq4::delimit<-EXTENDED_LIMIT,    i32sq4::REAL_V_MAX>;
    using extended_r_sq_t   = i32sq4::delimit<i32sq4::REAL_V_MIN, +EXTENDED_LIMIT>;
    using shifted_sq_l_t    = i32sq4::delimit<-EXTENDED_LIMIT,    +RESTRICTED_LIMIT>;
    using shifted_sq_r_t    = i32sq4::delimit<-RESTRICTED_LIMIT,  +EXTENDED_LIMIT>;

    ASSERT_TRUE((std::is_same_v< sq<int32_t, 4, -RESTRICTED_LIMIT,  +RESTRICTED_LIMIT >, restricted_sq_t >));
    ASSERT_TRUE((std::is_same_v< sq<int32_t, 4, -RESTRICTED_LIMIT,  i32sq4::REAL_V_MAX>, restricted_l_sq_t >));
    ASSERT_TRUE((std::is_same_v< sq<int32_t, 4, i32sq4::REAL_V_MIN, +RESTRICTED_LIMIT >, restricted_r_sq_t >));
    ASSERT_TRUE((std::is_same_v< sq<int32_t, 4, -EXTENDED_LIMIT,    +EXTENDED_LIMIT   >, extended_sq_t >));
    ASSERT_TRUE((std::is_same_v< sq<int32_t, 4, -EXTENDED_LIMIT,    i32sq4::REAL_V_MAX>, extended_l_sq_t >));
    ASSERT_TRUE((std::is_same_v< sq<int32_t, 4, i32sq4::REAL_V_MIN, +EXTENDED_LIMIT   >, extended_r_sq_t >));
    ASSERT_TRUE((std::is_same_v< sq<int32_t, 4, -EXTENDED_LIMIT,    +RESTRICTED_LIMIT >, shifted_sq_l_t >));
    ASSERT_TRUE((std::is_same_v< sq<int32_t, 4, -RESTRICTED_LIMIT,  +EXTENDED_LIMIT   >, shifted_sq_r_t >));
}

TEST_F(SQTest_Construct, sq_from_real__constexpr_int16_positiveF__expected_value) {
    constexpr double REAL_VALUE = -2047.6;
    auto sqValue = i16sq4::from_real<REAL_VALUE>();

    EXPECT_TRUE((std::is_same_v<int16_t, decltype(sqValue.reveal())>));
    EXPECT_TRUE((std::is_same_v<int, decltype(sqValue.to_real<int>())>));
    EXPECT_TRUE((std::is_same_v<double, decltype(sqValue.to_real())>));

    constexpr int16_t RESULT_MEM_VALUE = -32761;
    ASSERT_EQ(RESULT_MEM_VALUE, sqValue.reveal());
    ASSERT_EQ(-2047, sqValue.to_real<int>());
    ASSERT_NEAR(REAL_VALUE, sqValue.to_real(), i16sq4::RESOLUTION);
}

TEST_F(SQTest_Construct, sq_from_scaled__constexpr_int16_positiveF__expected_value) {
    constexpr int16_t MEM_VALUE = 31686;
    auto a = i16sq4::from_scaled<+MEM_VALUE>();
    auto b = i16sq4::from_scaled<-MEM_VALUE>();

    constexpr double RESULT_REAL_VALUE = 1980.375;
    ASSERT_EQ(+MEM_VALUE, a.reveal());
    ASSERT_EQ(-MEM_VALUE, b.reveal());
    ASSERT_NEAR(+RESULT_REAL_VALUE, a.to_real(), i16sq4::RESOLUTION);
    ASSERT_NEAR(-RESULT_REAL_VALUE, b.to_real(), i16sq4::RESOLUTION);
}

TEST_F(SQTest_Construct, sq_copy_constructor__int16_someF__int16_sameF) {
    constexpr double REAL_VALUE_A = -1024.2;
    auto a = i16sq4::from_real<REAL_VALUE_A>();
    auto b = i16sq4::from_sq(a);

    ASSERT_NEAR(REAL_VALUE_A, b.to_real(), i16sq4::RESOLUTION);
}

TEST_F(SQTest_Construct, sq_move_constructor__int16_someF__int16_sameF) {
    constexpr double REAL_VALUE_A = -1024.2;
    auto a = i16sq4::from_real<REAL_VALUE_A>();
    i16sq4 b = std::move(a);

    ASSERT_NEAR(REAL_VALUE_A, b.to_real(), i16sq4::RESOLUTION);
}

TEST_F(SQTest_Construct, sq_upscale_copy_constructor__int16_someF__int16_largerF) {
    constexpr double REAL_VALUE_A = -1024.2;
    auto a = i32sq4::from_real<REAL_VALUE_A>();
    auto b = i32sq8::from_sq(a);

    // note: the representation error due to rounding is determined by the resolution of the initial
    //       sq4 type and does not change if the value is up-scaled to another sq type
    ASSERT_NEAR(REAL_VALUE_A, b.to_real(), i32sq4::RESOLUTION);
}

TEST_F(SQTest_Construct, sq_downscale_copy_constructor__int16_someF__int16_smallerF) {
    constexpr double REAL_VALUE_A = -1024.2;
    auto a = i32sq4::from_real<REAL_VALUE_A>();
    auto b = i32sqm2::from_sq(a);

    // note: for down-scaling, the representation error is at most the sum of the two resolutions
    //       before and after the scaling operation
    ASSERT_NEAR(REAL_VALUE_A, b.to_real(), i32sq4::RESOLUTION + i32sqm2::RESOLUTION);
}

// EOF
